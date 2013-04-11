/*
 * Copyright (C) 2010 by Joseph A. Marrero and Shrewd LLC. http://www.manvscode.com/
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
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "tstring.h"

boolean tstring_create( tstring_t *p_string, const tchar *s )
{
	assert( p_string );
	p_string->length = tstrlen( s );
	p_string->s      = (tchar *) malloc( sizeof(tchar) * tstring_size(p_string) );

	tstrncpy( p_string->s, s, p_string->length + 1 );

	return p_string->s != NULL;
}

void tstring_destroy( const tstring_t *p_string )
{
	assert( p_string );
	free( p_string->s );
}

int tstring_compare( const tstring_t *left, const tstring_t *right )
{
	assert( left && right );
	return tstrncmp( left->s, right->s, left->length );
}

int tstring_ncompare( const tstring_t *left, const tstring_t *right, size_t n )
{
	assert( left && right );
	return tstrncmp( left->s, right->s, n );
}

int tstring_casecompare( const tstring_t *left, const tstring_t *right )
{
	assert( left && right );
	return tstrncasecmp( left->s, right->s, left->length );
}

int tstring_ncasecompare( const tstring_t *left, const tstring_t *right, size_t n )
{
	assert( left && right );
	return tstrncasecmp( left->s, right->s, n );
}

void tstring_tolower( tstring_t *p_string )
{
	tchar *p_s;

	assert( p_string );
	p_s = p_string->s;

	while( p_s )
	{
		*p_s = tchar_lower( *p_s );
		p_s++;
	}
}

void tstring_toupper( tstring_t *p_string )
{
	tchar *p_s;

	assert( p_string );
	p_s = p_string->s;

	while( p_s )
	{
		*p_s = tchar_upper( *p_s );
		p_s++;
	}
}

boolean tstring_copy( tstring_t *p_result, const tstring_t *p_string )
{
	assert( p_result && p_string );

	p_result->length = tstring_length( p_string );
	p_result->s      = (tchar *) realloc( p_result->s, tstring_size( p_string ) );

	if( p_result->s )
	{
		tstrncpy(
			tstring_string( p_result ),
			tstring_string( p_string ),
			tstring_length( p_string ) + 1
		);
		return TRUE;
	}

	return FALSE;
}

boolean tstring_ncopy( tstring_t *p_result, const tstring_t *p_string, size_t n )
{
	assert( p_result && p_string );
	assert( n > 0 && n <= tstring_length( p_string ) );

	p_result->length = tstring_length( p_string );
	p_result->s      = (tchar *) realloc( p_result->s, (1 + n) * sizeof(tchar) );

	if( p_result->s )
	{
		tstrncpy(
			tstring_string( p_result ),
			tstring_string( p_string ),
			n
		);
		return TRUE;
	}

	return FALSE;
}

boolean tstring_assign( tstring_t *p_result, const tchar *p_string )
{
	assert( p_result && p_string );

	p_result->length = tstrlen( p_string );
	p_result->s      = (tchar *) realloc( p_result->s, tstring_length( p_result ) + 1 );

	if( p_result->s )
	{
		tstrncpy(
			tstring_string( p_result ),
			p_string,
			tstring_length( p_result ) + 1
		);
		return TRUE;
	}

	return FALSE;
}

boolean tstring_concatenate( tstring_t *p_result, const tstring_t *p_string )
{
	size_t size;
	assert( p_result && p_string );

	size        = tstring_length( p_result ) + tstring_length( p_string ) + 1;
	p_result->s = (tchar *) realloc( p_result->s, size );

	if( p_result->s )
	{
		tstrncat(
			tstring_string( p_result ),
			tstring_string( p_string ),
			tstring_length( p_string )
		);
		return TRUE;
	}

	return FALSE;
}

boolean tstring_nconcatenate( tstring_t *p_result, const tstring_t *p_string, size_t n )
{
	size_t size;
	assert( p_result && p_string );

	size        = tstring_size( p_result ) + sizeof(tchar) * (n + 1);
	p_result->s = (tchar *) realloc( p_result->s, size );

	if( p_result->s )
	{
		tstrncat( tstring_string( p_result ), tstring_string( p_string ), n );
		return TRUE;
	}

	return FALSE;
}

boolean tstring_sconcatenate( tstring_t *p_string, const tchar *s )
{
	size_t size;
	size_t len;

	assert( p_string && s );
	len = tstrlen( s );

	size        = tstring_size( p_string ) + sizeof(tchar) * (len + 1);
	p_string->s = (tchar *) realloc( p_string->s, size );

	if( p_string->s )
	{
		tstrncat( tstring_string(p_string), s, len );
		return TRUE;
	}

	return FALSE;
}

void tstring_format( tstring_t *p_string, const tchar *format, ... )
{
	va_list args;
	size_t buffer_length;

	assert( p_string );
	assert( format );

	va_start( args, format );
	buffer_length = tvsnprintf( NULL, 0, format, args );
	va_end( args );

	if( buffer_length > 0 )
	{
		if( tstring_size(p_string) < buffer_length )
		{
			free( p_string->s );
			p_string->length = buffer_length;
			p_string->s      = (tchar *) malloc( sizeof(tchar) * tstring_size(p_string) );
		}

		va_start( args, format );
		tvsnprintf( p_string->s, buffer_length, format, args );
		p_string->s[ buffer_length ] = '\0';
		va_end( args );
	}
}

size_t tstring_ltrim( tstring_t *p_string )
{
	size_t charsRemoved = 0;
	assert( p_string );
	charsRemoved  = ltrim( p_string->s, _T("\n\r\t ") );
	p_string->length -= charsRemoved;
	return charsRemoved;
}

size_t tstring_rtrim( tstring_t *p_string )
{
	size_t charsRemoved = 0;
	assert( p_string );
	charsRemoved = rtrim( p_string->s, _T("\n\r\t ") );
	p_string->length -= charsRemoved;
	return charsRemoved;
}

size_t tstring_trim( tstring_t *p_string )
{
	return tstring_rtrim( p_string ) + tstring_ltrim( p_string );
}

size_t ltrim( tchar* s, const tchar* delimeters )
{
	tchar* start = s;
	assert( s );

	while( *s && tstrchr(delimeters, *s) )
	{
		++s;
	}

	tstrcpy( start, s );
	return s - start; /* # of chars removed */
}

size_t rtrim( tchar* s, const tchar* delimeters )
{
	tchar *end = s + tstrlen(s) - 1;
	tchar *new_end = end;
	assert( s );

	if( s != NULL )
	{
		while( tstrchr(delimeters, *new_end) && new_end >= s )
		{
			--new_end;
		}
		*(new_end + 1) = '\0';
	}

	return end - new_end;
}

size_t trim( tchar* s, const tchar* delimeters )
{
	return ltrim( s, delimeters ) + rtrim( s, delimeters );
}

