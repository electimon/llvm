#ifndef QXL_INTERNAL_H
#define QXL_INTERNAL_H

#include "qnxlocale.h"
#include <limits.h>

_Static_assert(UINT_MAX == UINT32_MAX, "The musl UTF-8 state machine requires 32-bit unsigned");

#define QXL_MB_LEN_MAX 4
#define bittab qxl_utf8_bittab
extern const uint32_t bittab[];

/* UTF-8 state machine constants from musl 1.2.5 internal.h. */
#define OOB(c,b) (((((b)>>3)-0x10)|(((b)>>3)+((int32_t)(c)>>26))) & ~7)
#define R(a,b) ((uint32_t)((a==0x80 ? 0x40u-b : 0u-a) << 23))
#define FAILSTATE R(0x80,0x80)
#define SA 0xc2u
#define SB 0xf4u
#define CODEUNIT(c) (0xdfff & (signed char)(c))
#define IS_CODEUNIT(c) ((unsigned)(c)-0xdf80 < 0x80)

struct qxl_locale { unsigned categories; int allocated; };
struct qxl_thread {
  qxl_locale_t locale;
  qxl_mbstate_t decode, length, encode, decode_string, encode_string;
};
struct qxl_thread *qxl_thread_get(int create);
int qxl_is_utf8(qxl_locale_t locale);
size_t qxl_decode(wchar_t *, const char *, size_t, qxl_mbstate_t *, int);
size_t qxl_encode(char *, wchar_t, qxl_mbstate_t *, int);
size_t qxl_encode_string(char *, const wchar_t **, size_t, size_t, qxl_mbstate_t *, int);

#endif
