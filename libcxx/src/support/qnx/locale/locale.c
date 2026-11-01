/* QNX adapter for musl's immutable builtins and per-thread locale selection.
 * Modelled on musl 1.2.5 newlocale.c/uselocale.c; see vendor/musl/COPYRIGHT. */
#include "internal.h"
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>

static struct qxl_locale c_locale = {0, 0};
static struct qxl_locale utf8_locale = {QXL_LC_ALL_MASK, 0};
static atomic_uint global_categories;
static pthread_key_t thread_key;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;
static int key_error;

static void make_key(void) { key_error = pthread_key_create(&thread_key, free); }

struct qxl_thread *qxl_thread_get(int create)
{
  int error = pthread_once(&key_once, make_key);
  if (error || key_error) { errno = error ? error : key_error; return NULL; }
  struct qxl_thread *thread = pthread_getspecific(thread_key);
  if (!thread && create) {
    thread = calloc(1, sizeof *thread);
    if (!thread) { errno = ENOMEM; return NULL; }
    thread->locale = QXL_GLOBAL_LOCALE;
    error = pthread_setspecific(thread_key, thread);
    if (error) { free(thread); errno = error; return NULL; }
  }
  return thread;
}

qxl_locale_t qxl_c_locale(void) { return &c_locale; }
qxl_locale_t qxl_utf8_locale(void) { return &utf8_locale; }

static unsigned categories(qxl_locale_t locale)
{
  if (!locale) {
    struct qxl_thread *thread = qxl_thread_get(0);
    locale = thread ? thread->locale : QXL_GLOBAL_LOCALE;
  }
  return locale == QXL_GLOBAL_LOCALE ? atomic_load_explicit(&global_categories, memory_order_relaxed)
                                    : locale->categories;
}

int qxl_is_utf8(qxl_locale_t locale) { return !!(categories(locale) & QXL_LC_CTYPE_MASK); }
size_t qxl_mb_cur_max_l(qxl_locale_t locale) { return qxl_is_utf8(locale) ? 4 : 1; }

static int encoding(const char *name)
{
  if (!strcmp(name, "C") || !strcmp(name, "POSIX")) return 0;
  if (!strcmp(name, "C.UTF-8") || !strcmp(name, "C.utf8")) return 1;
  errno = ENOENT;
  return -1;
}

qxl_locale_t qxl_newlocale(unsigned mask, const char *name, qxl_locale_t base)
{
  if (!name || (mask & ~QXL_LC_ALL_MASK) || base == QXL_GLOBAL_LOCALE) {
    errno = EINVAL; return NULL;
  }
  unsigned result = base ? base->categories : 0;
  static const char *const variables[] = {
    "LC_COLLATE", "LC_CTYPE", "LC_MONETARY", "LC_NUMERIC", "LC_TIME", "LC_MESSAGES"
  };
  for (unsigned i = 0; i < 6; ++i) {
    unsigned bit = 1u << i;
    if (!(mask & bit)) continue;
    const char *selected = name;
    if (!*selected) {
      selected = getenv("LC_ALL");
      if (!selected || !*selected) selected = getenv(variables[i]);
      if (!selected || !*selected) selected = getenv("LANG");
      if (!selected || !*selected) selected = "C";
    }
    int utf8 = encoding(selected);
    if (utf8 < 0) return NULL; /* Preserve base on failure. */
    result = utf8 ? result | bit : result & ~bit;
  }
  if (base && base->allocated) { base->categories = result; return base; }
  if (!result) return &c_locale;
  if (result == QXL_LC_ALL_MASK) return &utf8_locale;
  qxl_locale_t locale = malloc(sizeof *locale);
  if (!locale) { errno = ENOMEM; return NULL; }
  *locale = (struct qxl_locale){result, 1};
  return locale;
}

qxl_locale_t qxl_duplocale(qxl_locale_t locale)
{
  if (!locale) { errno = EINVAL; return NULL; }
  qxl_locale_t copy = malloc(sizeof *copy);
  if (!copy) { errno = ENOMEM; return NULL; }
  *copy = (struct qxl_locale){categories(locale), 1};
  return copy;
}

void qxl_freelocale(qxl_locale_t locale)
{
  if (locale && locale != QXL_GLOBAL_LOCALE && locale->allocated) free(locale);
}

qxl_locale_t qxl_uselocale(qxl_locale_t locale)
{
  struct qxl_thread *thread = qxl_thread_get(locale != NULL);
  if (!thread) {
    if (locale || key_error) return NULL;
    return QXL_GLOBAL_LOCALE;
  }
  qxl_locale_t previous = thread->locale;
  if (locale) thread->locale = locale;
  return previous;
}

int qxl_setglobal(qxl_locale_t locale)
{
  if (!locale) { errno = EINVAL; return -1; }
  atomic_store_explicit(&global_categories, categories(locale), memory_order_relaxed);
  return 0;
}
