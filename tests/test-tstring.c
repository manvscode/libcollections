#include <locale.h>
#include <assert.h>
#ifndef UNICODE
#define UNICODE /* TODO: Fix this. */
#endif

#ifndef UNICODE
#error "These tests assume unicode support"
#endif
#include "tstring.h"

int main( int argc, char *argv[] )
{
	char  ascii[ 128 ];
	tchar unicode[ 128 ]; 

	TERM_BEG(ascii);
	TERM_BEG(unicode);

	if( !setlocale(LC_CTYPE, "en_US.utf-8") )
	{
		fprintf(stderr, "Can't set the specified locale! "
				"Check LANG, LC_CTYPE, LC_ALL.\n");
		return 1;
	}


	tstrcat( unicode, _T("Hello") );
	tstrcat( unicode, _T(" ") );
	tstrcat( unicode, _T("World!") );
	tprintf( _T("string concatenation test: %S\n"), tstrcmp( unicode, _T("Hello World!") ) == 0 ? _T("passed") : _T("failed") );


	unicode_to_ascii( ascii, unicode, sizeof(ascii) );
	TERM_END( ascii );
	tprintf( _T("unicode to ascii test: %S\n"), strcmp( ascii, "Hello World!" ) == 0 ? _T("passed") : _T("failed") );


	const tchar *russian_text_utf8  = _T("В России тест принадлежит вам.");
	tprintf( _T(" UTF-8 Russian text = %S\n"), russian_text_utf8 );

	//const tchar *russian_text_utf16 = _T("");
	//tprintf( _T("UTF-16 Russian text = %S\n"), russian_text_utf16 );

	return 0;
}
