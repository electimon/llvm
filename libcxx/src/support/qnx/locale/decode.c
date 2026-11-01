/* Adapted from musl 1.2.5; see vendor/musl/COPYRIGHT. */
#include <stdlib.h>
#include <wchar.h>
#include <errno.h>
#include "internal.h"

size_t qxl_decode(wchar_t *restrict wc, const char *restrict src, size_t n, qxl_mbstate_t *restrict st, int utf8)
{
	unsigned c;
	const unsigned char *s = (const void *)src;
	const size_t N = n;
	wchar_t dummy;

	c = st->pending;

	if (!s) {
		if (c) goto ilseq;
		return 0;
	} else if (!wc) wc = &dummy;

	if (!n) return -2;
	if (!c) {
		if (*s < 0x80) return !!(*wc = *s);
		if (!utf8) return (*wc = CODEUNIT(*s)), 1;
		if (*s-SA > SB-SA) goto ilseq;
		c = bittab[*s++-SA]; n--;
	}

	if (n) {
		if (OOB(c,*s)) goto ilseq;
loop:
		c = (c<<6) | (*s++-0x80); n--;
		if (!(c&(1U<<31))) {
			st->pending = 0;
			*wc = c;
			return N-n;
		}
		if (n) {
			if (*s-0x80u >= 0x40) goto ilseq;
			goto loop;
		}
	}

	st->pending = c;
	return -2;
ilseq:
	st->pending = 0;
	errno = EILSEQ;
	return -1;
}
