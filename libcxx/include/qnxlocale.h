#ifndef QNXLOCALE_H
#define QNXLOCALE_H

#include <stddef.h>
#include <stdint.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct qxl_locale *qxl_locale_t;
typedef struct { uint32_t pending; } qxl_mbstate_t;

/* Category masks match the QNX category bits, not musl's category numbers. */
#define QXL_LC_COLLATE_MASK  1u
#define QXL_LC_CTYPE_MASK    2u
#define QXL_LC_MONETARY_MASK 4u
#define QXL_LC_NUMERIC_MASK  8u
#define QXL_LC_TIME_MASK     16u
#define QXL_LC_MESSAGES_MASK 32u
#define QXL_LC_ALL_MASK     63u
#define QXL_GLOBAL_LOCALE ((qxl_locale_t)(intptr_t)-1)

qxl_locale_t qxl_c_locale(void);
qxl_locale_t qxl_utf8_locale(void);
qxl_locale_t qxl_newlocale(unsigned mask, const char *name, qxl_locale_t base);
qxl_locale_t qxl_duplocale(qxl_locale_t locale);
void qxl_freelocale(qxl_locale_t locale);
/* NULL queries selection; non-NULL selects and returns the previous handle. */
qxl_locale_t qxl_uselocale(qxl_locale_t locale);
/* Copy category settings into this library's global fallback; initially C. */
int qxl_setglobal(qxl_locale_t locale);
/* For all *_l functions, NULL means the calling thread's current selection. */
size_t qxl_mb_cur_max_l(qxl_locale_t locale);
int qxl_mbsinit(const qxl_mbstate_t *state);

size_t qxl_mbrtowc_l(wchar_t *, const char *, size_t, qxl_mbstate_t *, qxl_locale_t);
size_t qxl_mbrlen_l(const char *, size_t, qxl_mbstate_t *, qxl_locale_t);
size_t qxl_wcrtomb_l(char *, wchar_t, qxl_mbstate_t *, qxl_locale_t);
size_t qxl_wcsnrtombs_l(char *, const wchar_t **, size_t, size_t, qxl_mbstate_t *, qxl_locale_t);
size_t qxl_mbsnrtowcs_l(wchar_t *, const char **, size_t, size_t, qxl_mbstate_t *, qxl_locale_t);
size_t qxl_wcsrtombs_l(char *, const wchar_t **, size_t, qxl_mbstate_t *, qxl_locale_t);
size_t qxl_mbsrtowcs_l(wchar_t *, const char **, size_t, qxl_mbstate_t *, qxl_locale_t);
wint_t qxl_btowc_l(int, qxl_locale_t);
int qxl_wctob_l(wint_t, qxl_locale_t);

#ifdef __cplusplus
}
#endif
#endif
