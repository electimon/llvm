/* Adapted from musl 1.2.5; see vendor/musl/COPYRIGHT. */
#include "internal.h"
#include <limits.h>
#include <string.h>

size_t qxl_encode_string(char *restrict dst, const wchar_t **restrict wcs, size_t wn, size_t n, qxl_mbstate_t *restrict st, int utf8)
{
	const wchar_t *ws = *wcs;
	size_t cnt = 0;
	if (!dst) n=0;
	while (ws && wn && (!dst || n)) {
		char tmp[QXL_MB_LEN_MAX];
		size_t l = qxl_encode(n<QXL_MB_LEN_MAX ? tmp : dst, *ws, st, utf8);
		if (l==(size_t)-1) {
			cnt = -1;
			break;
		}
		if (dst) {
			if (n<QXL_MB_LEN_MAX) {
				if (l>n) break;
				memcpy(dst, tmp, l);
			}
			dst += l;
			n -= l;
		}
		if (!*ws) {
			ws = 0;
			break;
		}
		ws++;
		wn--;
		cnt += l;
	}
	if (dst) *wcs = ws;
	return cnt;
}
