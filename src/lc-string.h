/*
 * Copyright (C) 2010 by Joseph A. Marrero.  http://www.manvscode.com/
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>
#ifndef _LC_STRING_H_
#define _LC_STRING_H_

#ifdef __cplusplus
extern "C" {
#endif


#if defined(WIN32) || defined(WIN64)
#if defined(UNICODE)
	#define wcsncmp     _wcsncmp
	#define wcscasecmp  _wcsnicmp
#else
	#define strcasecmp  _stricmp
	#define strncasecmp _strnicmp
#endif
#endif

#ifndef wcsdup
wchar_t* wcsdup( const wchar_t* s );
#endif

#if defined(UNICODE)
	typedef wchar_t      lc_char_t;
	#ifndef TEXT
	#define TEXT(str)    L##str
	#endif
	#define lc_strdup      wcsdup
	#define lc_strlen      wcslen
	#define lc_strcmp      wcscmp
	#define lc_strcoll     wcscoll
	#define lc_strncmp     wcsncmp
	#define lc_strcasecmp  wcscasecmp
	#define lc_strncasecmp wcsncasecmp
	#define lc_strcpy      wcscpy
	#define lc_strncpy     wcsncpy
	#define lc_strcat      wcscat
	#define lc_strncat     wcsncat
	#define lc_strspn      wcsspn
	#define lc_strcspn     wcscspn
	#define lc_strpbrk     wcspbrk
	#define lc_strtok      wcstok
	#define lc_strchr      wcschr
	#define lc_strstr      wcsstr
	#define lc_char_lower  towlower
	#define lc_char_upper  towupper
	#define lc_printf      wprintf
	#define lc_fprintf     fwprintf
	#define lc_sprintf     #error "There is no wide character equivalent to vsprintf"
	#define lc_snprintf    swprintf
	#define lc_vprintf     vwprintf
	#define lc_vfprintf    vfwprintf
	#define lc_vsprintf    vswprintf
	#define lc_vsnprintf   vswprintf /* intentionally missing n */
	#define tmemcpy      wmemcpy
	#if defined(WIN32)
		#define _tmain       wmain  /* main() is wmain() */
	#else
		#define _tmain       main
	#endif
#else
	typedef char         lc_char_t;
	#ifndef TEXT
	#define TEXT(str)    str
	#endif
	#define lc_strdup      strdup
	#define lc_strlen      strlen
	#define lc_strcmp      strcmp
	#define lc_strcoll     strcoll
	#define lc_strncmp     strncmp
	#define lc_strcasecmp  strcasecmp
	#define lc_strncasecmp strncasecmp
	#define lc_strcpy      strcpy
	#define lc_strncpy     strncpy
	#define lc_strcat      strcat
	#define lc_strncat     strncat
	#define lc_strspn      strspn
	#define lc_strcspn     strcspn
	#define lc_strpbrk     strpbrk
	#define lc_strtok      strtok
	#define lc_strchr      strchr
	#define lc_strstr      strstr
	#define lc_char_lower  tolower
	#define lc_char_upper  toupper
	#define lc_printf      printf
	#define lc_fprintf     fprintf
	#define lc_sprintf     sprintf
	#define lc_snprintf    snprintf
	#define lc_vprintf     vprintf
	#define lc_vfprintf    vfprintf
	#define lc_vsprintf    vsprintf
	#define lc_vsnprintf   vsnprintf
	#define tmemcpy      memcpy
	#define _tmain       main
#endif



#define ascii_to_unicode     mbsrtowcs /*  char* to wchar_t*  */
#define unicode_to_ascii     wcstombs  /*  wchar_t* to char*  */

#ifndef _T
#define _T(str)          TEXT(str)
#endif
#define LC_TERM_BEG(str)    (str[0] = '\0')
#define LC_TERM_END(str)    (str[sizeof(str) - 1] = '\0')

typedef struct lc_string {
	size_t length;
	lc_char_t* s;
} lc_string_t;


bool    lc_string_create       ( lc_string_t* p_string, const lc_char_t *s );
void    lc_string_destroy      ( const lc_string_t* p_string );
int     lc_string_compare      ( const lc_string_t* left, const lc_string_t* right );
int     lc_string_ncompare     ( const lc_string_t* left, const lc_string_t* right, size_t n );
int     lc_string_casecompare  ( const lc_string_t* left, const lc_string_t* right );
int     lc_string_ncasecompare ( const lc_string_t* left, const lc_string_t* right, size_t n );
void    lc_string_tolower      ( lc_string_t* p_string );
void    lc_string_toupper      ( lc_string_t* p_string );
bool    lc_string_copy         ( lc_string_t* p_result, const lc_string_t* p_string );
bool    lc_string_ncopy        ( lc_string_t* p_result, const lc_string_t* p_string, size_t n );
bool    lc_string_assign       ( lc_string_t* p_result, const lc_char_t *p_string );
bool    lc_string_concatenate  ( lc_string_t* p_result, const lc_string_t* s );
bool    lc_string_nconcatenate    ( lc_string_t* p_result, const lc_string_t* p_string, size_t n );
bool    lc_string_sconcatenate    ( lc_string_t* p_string, const lc_char_t *s );
void    lc_string_format       ( lc_string_t* p_string, const lc_char_t *format, ... );
size_t  lc_string_rtrim        ( lc_string_t* p_string );
size_t  lc_string_ltrim        ( lc_string_t* p_string );
size_t  lc_string_trim         ( lc_string_t* p_string );

#define lc_string_get( p_string )            ((p_string)->s)
#define lc_string_string( p_string )         ((p_string)->s)
#define lc_string_length( p_string )         ((p_string)->length)
#define lc_string_size( p_string )           (sizeof(lc_char_t) * ((p_string)->length + 1))

size_t lc_ltrim   ( lc_char_t* s, const lc_char_t* delimeters );
size_t lc_rtrim   ( lc_char_t* s, const lc_char_t* delimeters );
size_t lc_trim    ( lc_char_t* s, const lc_char_t* delimeters );
lc_char_t* lc_strtolower ( lc_char_t* s );
lc_char_t* lc_strtoupper ( lc_char_t* s );

#ifdef __cplusplus
} /* external C linkage */
#endif
#endif /* _LC_STRING_H_ */
