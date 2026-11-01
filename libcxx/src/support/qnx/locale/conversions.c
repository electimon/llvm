#include "internal.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>

int qxl_mbsinit(const qxl_mbstate_t *state) { return !state || !state->pending; }

size_t qxl_mbrtowc_l(wchar_t *wc, const char *src, size_t n, qxl_mbstate_t *state, qxl_locale_t locale)
{
  if (!state) {
    struct qxl_thread *thread = qxl_thread_get(1);
    if (!thread) return (size_t)-1;
    state = &thread->decode;
  }
  return qxl_decode(wc, src, n, state, qxl_is_utf8(locale));
}

size_t qxl_mbrlen_l(const char *src, size_t n, qxl_mbstate_t *state, qxl_locale_t locale)
{
  if (!state) {
    struct qxl_thread *thread = qxl_thread_get(1);
    if (!thread) return (size_t)-1;
    state = &thread->length;
  }
  return qxl_decode(NULL, src, n, state, qxl_is_utf8(locale));
}

size_t qxl_wcrtomb_l(char *dst, wchar_t wc, qxl_mbstate_t *state, qxl_locale_t locale)
{
  if (!state) {
    struct qxl_thread *thread = qxl_thread_get(1);
    if (!thread) return (size_t)-1;
    state = &thread->encode;
  }
  return qxl_encode(dst, wc, state, qxl_is_utf8(locale));
}

size_t qxl_wcsnrtombs_l(char *dst, const wchar_t **src, size_t nwc, size_t len,
                       qxl_mbstate_t *state, qxl_locale_t locale)
{
  if (!state) {
    struct qxl_thread *thread = qxl_thread_get(1);
    if (!thread) return (size_t)-1;
    state = &thread->encode_string;
  }
  qxl_mbstate_t query = *state;
  return qxl_encode_string(dst, src, nwc, len, dst ? state : &query, qxl_is_utf8(locale));
}

/* Adapted bounded decoder around musl's mbrtowc state machine. Unlike the
 * older musl bulk loop, consume partial input and retain restartable state. */
size_t qxl_mbsnrtowcs_l(wchar_t *dst, const char **src, size_t nms, size_t len,
                       qxl_mbstate_t *state, qxl_locale_t locale)
{
  if (!state) {
    struct qxl_thread *thread = qxl_thread_get(1);
    if (!thread) return (size_t)-1;
    state = &thread->decode_string;
  }
  qxl_mbstate_t query = *state;
  if (!dst) state = &query;
  const char *next = *src;
  size_t count = 0;
  int utf8 = qxl_is_utf8(locale);
  while (nms && (!dst || count < len)) {
    wchar_t wc;
    size_t n = qxl_decode(&wc, next, nms, state, utf8);
    if (n == (size_t)-1) { if (dst) *src = next; return n; }
    if (n == (size_t)-2) { next += nms; break; }
    if (dst) dst[count] = wc;
    if (!n) { if (dst) *src = NULL; return count; }
    next += n;
    nms -= n;
    ++count;
  }
  if (dst) *src = next;
  return count;
}

size_t qxl_wcsrtombs_l(char *dst, const wchar_t **src, size_t len, qxl_mbstate_t *state, qxl_locale_t locale)
{ return qxl_wcsnrtombs_l(dst, src, SIZE_MAX, len, state, locale); }

size_t qxl_mbsrtowcs_l(wchar_t *dst, const char **src, size_t len, qxl_mbstate_t *state, qxl_locale_t locale)
{ return qxl_mbsnrtowcs_l(dst, src, SIZE_MAX, len, state, locale); }

wint_t qxl_btowc_l(int c, qxl_locale_t locale)
{
  if (c == EOF || (unsigned)c > UCHAR_MAX) return WEOF;
  char byte = (char)c;
  wchar_t wc;
  qxl_mbstate_t state = {0};
  size_t n = qxl_mbrtowc_l(&wc, &byte, 1, &state, locale);
  return n == (size_t)-1 || n == (size_t)-2 ? WEOF : (wint_t)wc;
}

int qxl_wctob_l(wint_t wc, qxl_locale_t locale)
{
  char bytes[QXL_MB_LEN_MAX];
  qxl_mbstate_t state = {0};
  return qxl_wcrtomb_l(bytes, (wchar_t)wc, &state, locale) == 1 ? (unsigned char)bytes[0] : EOF;
}
