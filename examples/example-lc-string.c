/*
 * Copyright (C) 2010-2022 by Joseph A. Marrero.  https://joemarrero.com/
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
#include <locale.h>
#include <assert.h>
#include <lc-string.h>

#ifndef UNICODE
#error "The lc_string example assume unicode and C99 support"
#endif

int main( int argc, char *argv[] )
{
	char  ascii[ 128 ];
	lc_char_t unicode[ 128 ];

	LC_TERM_BEG(ascii);
	LC_TERM_BEG(unicode);

	if( !setlocale(LC_CTYPE, "en_US.utf-8") )
	{
		fprintf(stderr, "Can't set the specified locale! "
				"Check LANG, LC_CTYPE, LC_ALL.\n");
		return 1;
	}


	lc_strcat( unicode, _T("Hello") );
	lc_strcat( unicode, _T(" ") );
	lc_strcat( unicode, _T("World!") );
	lc_printf( _T("string concatenation test: %S\n"), lc_strcmp( unicode, _T("Hello World!") ) == 0 ? _T("passed") : _T("failed") );


	unicode_to_ascii( ascii, unicode, sizeof(ascii) );
	LC_TERM_END( ascii );
	lc_printf( _T("unicode to ascii test: %S\n"), strcmp( ascii, "Hello World!" ) == 0 ? _T("passed") : _T("failed") );


	const lc_char_t* russian_text_utf8  = _T("В России тест принадлежит вам.");
	lc_printf( _T(" UTF-8 Russian text = %S\n"), russian_text_utf8 );

	lc_string_t ltext;
	lc_string_t rtext;
	lc_string_create( &ltext, _T("   \t\n \r This is a test of ltrim().") );
	lc_string_create( &rtext, _T("This is a test of rtrim().   \n\t\t \r ") );

	size_t n = 0;

	n = lc_string_ltrim( &ltext );
	assert( n == 8 );
	lc_printf( _T("[%S] -- %ld chars removed\n"), ltext.s, n );

	n = lc_string_rtrim( &rtext );
	assert( n == 9 );
	lc_printf( _T("[%S] -- %ld chars removed\n"), rtext.s, n );

	lc_string_destroy( &ltext );
	lc_string_destroy( &rtext );


	return 0;
}
