//===-----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___LOCALE_DIR_SUPPORT_QNX_H
#define _LIBCPP___LOCALE_DIR_SUPPORT_QNX_H

#include <__config>
#include <__cstddef/size_t.h>
#include <__std_mbstate_t.h>
#include <__utility/forward.h>
#include <clocale> // std::lconv
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if _LIBCPP_HAS_WIDE_CHARACTERS
#  include <wchar.h>
#  include <wctype.h>
#endif

#if __has_include(<xlocale.h>)
#  include <xlocale.h>
#endif

#include <qnxlocale.h>
#include <errno.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD
namespace __locale {

//
// Locale management
//
#define _LIBCPP_COLLATE_MASK QXL_LC_COLLATE_MASK
#define _LIBCPP_CTYPE_MASK QXL_LC_CTYPE_MASK
#define _LIBCPP_MONETARY_MASK QXL_LC_MONETARY_MASK
#define _LIBCPP_NUMERIC_MASK QXL_LC_NUMERIC_MASK
#define _LIBCPP_TIME_MASK QXL_LC_TIME_MASK
#define _LIBCPP_MESSAGES_MASK QXL_LC_MESSAGES_MASK
#define _LIBCPP_ALL_MASK QXL_LC_ALL_MASK
#define _LIBCPP_LC_ALL LC_ALL

using __locale_t _LIBCPP_NODEBUG = ::qxl_locale_t;
#if defined(_LIBCPP_BUILDING_LIBRARY)
using __lconv_t _LIBCPP_NODEBUG = std::lconv;

inline _LIBCPP_HIDE_FROM_ABI __locale_t __newlocale(int __category_mask, const char* __locale, __locale_t __base) {
  return ::qxl_newlocale(__category_mask, __locale, __base);
}

inline _LIBCPP_HIDE_FROM_ABI void __freelocale(__locale_t __loc) { ::qxl_freelocale(__loc); }

inline _LIBCPP_HIDE_FROM_ABI char* __setlocale(int __category, char const* __locale) {
  return ::setlocale(__category, __locale);
}

inline __lconv_t* __localeconv(__locale_t& __loc) {
  return ::localeconv();
}
#endif // _LIBCPP_BUILDING_LIBRARY

inline __locale_t __get_c_locale() {
  return 0;
}

//
// Strtonum functions
//
template <class _FloatT>
_LIBCPP_HIDE_FROM_ABI _FloatT __str_to_float_c_locale(const char* __nptr, char** __endptr);

template <>
inline _LIBCPP_HIDE_FROM_ABI float __str_to_float_c_locale<float>(const char* __nptr, char** __endptr) {
  return ::strtof(__nptr, __endptr);
}

template <>
inline _LIBCPP_HIDE_FROM_ABI double __str_to_float_c_locale<double>(const char* __nptr, char** __endptr) {
  return ::strtod(__nptr, __endptr);
}

template <>
inline _LIBCPP_HIDE_FROM_ABI long double __str_to_float_c_locale<long double>(const char* __nptr, char** __endptr) {
  return ::strtold(__nptr, __endptr);
}

//
// Character manipulation functions
//
#if defined(_LIBCPP_BUILDING_LIBRARY)
inline _LIBCPP_HIDE_FROM_ABI int __toupper(int __c, __locale_t __loc) { return ::toupper(__c); }

inline _LIBCPP_HIDE_FROM_ABI int __tolower(int __c, __locale_t __loc) { return ::tolower(__c); }

inline _LIBCPP_HIDE_FROM_ABI int __strcoll(const char* __s1, const char* __s2, __locale_t __loc) {
  return ::strcoll(__s1, __s2);
}

inline _LIBCPP_HIDE_FROM_ABI size_t __strxfrm(char* __dest, const char* __src, size_t __n, __locale_t __loc) {
  return ::strxfrm(__dest, __src, __n);
}

#  if _LIBCPP_HAS_WIDE_CHARACTERS
inline _LIBCPP_HIDE_FROM_ABI int __iswctype(wint_t __c, wctype_t __type, __locale_t __loc) {
  return ::iswctype(__c, __type);
}

inline _LIBCPP_HIDE_FROM_ABI int __iswspace(wint_t __c, __locale_t __loc) { return ::iswspace(__c); }

inline _LIBCPP_HIDE_FROM_ABI int __iswprint(wint_t __c, __locale_t __loc) { return ::iswprint(__c); }

inline _LIBCPP_HIDE_FROM_ABI int __iswcntrl(wint_t __c, __locale_t __loc) { return ::iswcntrl(__c); }

inline _LIBCPP_HIDE_FROM_ABI int __iswupper(wint_t __c, __locale_t __loc) { return ::iswupper(__c); }

inline _LIBCPP_HIDE_FROM_ABI int __iswlower(wint_t __c, __locale_t __loc) { return ::iswlower(__c); }

inline _LIBCPP_HIDE_FROM_ABI int __iswalpha(wint_t __c, __locale_t __loc) { return ::iswalpha(__c); }

inline _LIBCPP_HIDE_FROM_ABI int __iswblank(wint_t __c, __locale_t __loc) { return ::iswblank(__c); }

inline _LIBCPP_HIDE_FROM_ABI int __iswdigit(wint_t __c, __locale_t __loc) { return ::iswdigit(__c); }

inline _LIBCPP_HIDE_FROM_ABI int __iswpunct(wint_t __c, __locale_t __loc) { return ::iswpunct(__c); }

inline _LIBCPP_HIDE_FROM_ABI int __iswxdigit(wint_t __c, __locale_t __loc) { return ::iswxdigit(__c); }

inline _LIBCPP_HIDE_FROM_ABI wint_t __towupper(wint_t __c, __locale_t __loc) { return ::towupper(__c); }

inline _LIBCPP_HIDE_FROM_ABI wint_t __towlower(wint_t __c, __locale_t __loc) { return ::towlower(__c); }

inline _LIBCPP_HIDE_FROM_ABI int __wcscoll(const wchar_t* __ws1, const wchar_t* __ws2, __locale_t __loc) {
  return ::wcscoll(__ws1, __ws2);
}

inline _LIBCPP_HIDE_FROM_ABI size_t __wcsxfrm(wchar_t* __dest, const wchar_t* __src, size_t __n, __locale_t __loc) {
  return ::wcsxfrm(__dest, __src, __n);
}
#  endif // _LIBCPP_HAS_WIDE_CHARACTERS

inline _LIBCPP_HIDE_FROM_ABI _LIBCPP_ATTRIBUTE_FORMAT(__strftime__, 3, 0) size_t
    __strftime(char* __s, size_t __max, const char* __format, const struct tm* __tm, __locale_t __loc) {
  return ::strftime(__s, __max, __format, __tm);
}

//
// Other functions
//
// libc++ uses nullptr as its classic-locale sentinel. qxl's nullptr instead
// means the current thread locale, so always resolve the sentinel explicitly.
inline _LIBCPP_HIDE_FROM_ABI __locale_t __qxl_locale(__locale_t __loc) {
  return __loc ? __loc : ::qxl_c_locale();
}

inline _LIBCPP_HIDE_FROM_ABI decltype(MB_CUR_MAX) __mb_len_max(__locale_t __loc) {
  return ::qxl_mb_cur_max_l(__qxl_locale(__loc));
}

#  if _LIBCPP_HAS_WIDE_CHARACTERS
// QNX mbsinit checks _State; musl's conversion engine keeps a 32-bit word.
// Copy the word rather than aliasing the two unrelated state types.
struct __qxl_state {
  mbstate_t* __native;
  ::qxl_mbstate_t __state;
  explicit __qxl_state(mbstate_t* __ps) : __native(__ps), __state{} {
    static_assert(sizeof(__ps->_Wchar) == sizeof(__state.pending), "QNX mbstate word size");
    if (__ps && __ps->_State)
      ::memcpy(&__state.pending, &__ps->_Wchar, sizeof(__state.pending));
  }
  ~__qxl_state() {
    if (__native) {
      ::memcpy(&__native->_Wchar, &__state.pending, sizeof(__state.pending));
      __native->_State = __state.pending != 0;
    }
  }
  ::qxl_mbstate_t* __get() { return __native ? &__state : nullptr; }
};

inline _LIBCPP_HIDE_FROM_ABI wint_t __btowc(int __c, __locale_t __loc) {
  return ::qxl_btowc_l(__c, __qxl_locale(__loc));
}
inline _LIBCPP_HIDE_FROM_ABI int __wctob(wint_t __c, __locale_t __loc) {
  return ::qxl_wctob_l(__c, __qxl_locale(__loc));
}

inline _LIBCPP_HIDE_FROM_ABI size_t __wcrtomb(char* __s, wchar_t __wc, mbstate_t* __ps, __locale_t __loc) {
  __qxl_state __state(__ps);
  return ::qxl_wcrtomb_l(__s, __wc, __state.__get(), __qxl_locale(__loc));
}

inline _LIBCPP_HIDE_FROM_ABI size_t __mbrtowc(wchar_t* __pwc, const char* __s, size_t __n, mbstate_t* __ps, __locale_t __loc) {
  __qxl_state __state(__ps);
  return ::qxl_mbrtowc_l(__pwc, __s, __n, __state.__get(), __qxl_locale(__loc));
}

inline _LIBCPP_HIDE_FROM_ABI size_t __mbrlen(const char* __s, size_t __n, mbstate_t* __ps, __locale_t __loc) {
  __qxl_state __state(__ps);
  return ::qxl_mbrlen_l(__s, __n, __state.__get(), __qxl_locale(__loc));
}

inline _LIBCPP_HIDE_FROM_ABI size_t __mbsrtowcs(wchar_t* __dest, const char** __src, size_t __len, mbstate_t* __ps, __locale_t __loc) {
  __qxl_state __state(__ps);
  return ::qxl_mbsrtowcs_l(__dest, __src, __len, __state.__get(), __qxl_locale(__loc));
}

inline _LIBCPP_HIDE_FROM_ABI size_t __wcsnrtombs(char* __dest, const wchar_t** __src, size_t __nwc, size_t __len, mbstate_t* __ps, __locale_t __loc) {
  __qxl_state __state(__ps);
  return ::qxl_wcsnrtombs_l(__dest, __src, __nwc, __len, __state.__get(), __qxl_locale(__loc));
}

inline _LIBCPP_HIDE_FROM_ABI size_t __mbsnrtowcs(wchar_t* __dest, const char** __src, size_t __nms, size_t __len, mbstate_t* __ps, __locale_t __loc) {
  __qxl_state __state(__ps);
  return ::qxl_mbsnrtowcs_l(__dest, __src, __nms, __len, __state.__get(), __qxl_locale(__loc));
}

inline _LIBCPP_HIDE_FROM_ABI int __mbtowc(wchar_t* __pwc, const char* __s, size_t __n, __locale_t __loc) {
  if (!__s)
    return 0; // Both supported encodings are stateless.
  ::qxl_mbstate_t __state = {};
  size_t __result = ::qxl_mbrtowc_l(__pwc, __s, __n, &__state, __qxl_locale(__loc));
  if (__result == static_cast<size_t>(-2)) {
    errno = EILSEQ;
    return -1;
  }
  return static_cast<int>(__result);
}
#  endif // _LIBCPP_HAS_WIDE_CHARACTERS

inline _LIBCPP_HIDE_FROM_ABI const char* __get_locale_encoding(__locale_t __loc) {
  return ::qxl_mb_cur_max_l(__qxl_locale(__loc)) == 4 ? "UTF-8" : nullptr;
}
#endif // _LIBCPP_BUILDING_LIBRARY

_LIBCPP_DIAGNOSTIC_PUSH
_LIBCPP_CLANG_DIAGNOSTIC_IGNORED("-Wgcc-compat")
_LIBCPP_GCC_DIAGNOSTIC_IGNORED("-Wformat-nonliteral") // GCC doesn't support [[gnu::format]] on variadic templates
#ifdef _LIBCPP_COMPILER_CLANG_BASED
#  define _LIBCPP_VARIADIC_ATTRIBUTE_FORMAT(...) _LIBCPP_ATTRIBUTE_FORMAT(__VA_ARGS__)
#else
#  define _LIBCPP_VARIADIC_ATTRIBUTE_FORMAT(...) /* nothing */
#endif

template <class... _Args>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_VARIADIC_ATTRIBUTE_FORMAT(__printf__, 4, 5) int __snprintf(
    char* __s, size_t __n, __locale_t, const char* __format, _Args&&... __args) {
  return ::snprintf(__s, __n, __format, std::forward<_Args>(__args)...);
}

template <class... _Args>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_VARIADIC_ATTRIBUTE_FORMAT(__printf__, 3, 4) int __asprintf(
    char** __s, __locale_t, const char* __format, _Args&&... __args) {
  *__s = nullptr;
  // QNX has no asprintf; snprintf reports the required size without the terminator.
  int __length = ::snprintf(nullptr, 0, __format, std::forward<_Args>(__args)...);
  if (__length < 0)
    return -1;

  size_t __size = static_cast<size_t>(__length) + 1;
  char* __buffer = static_cast<char*>(::malloc(__size));
  if (__buffer == nullptr)
    return -1;

  int __written = ::snprintf(__buffer, __size, __format, std::forward<_Args>(__args)...);
  if (__written < 0 || static_cast<size_t>(__written) >= __size) {
    ::free(__buffer);
    return -1;
  }

  *__s = __buffer;
  return __written;
}
_LIBCPP_DIAGNOSTIC_POP
#undef _LIBCPP_VARIADIC_ATTRIBUTE_FORMAT

} // namespace __locale
_LIBCPP_END_NAMESPACE_STD

#define _LIBCPP_PROVIDES_DEFAULT_RUNE_TABLE 1

#endif // _LIBCPP___LOCALE_DIR_SUPPORT_QNX_H
