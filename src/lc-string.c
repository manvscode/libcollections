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
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "lc-string.h"

bool lc_string_create( lc_string_t* p_string, const lc_char_t *s )
{
	assert( p_string );
	p_string->length = lc_strlen( s );
	p_string->s      = (lc_char_t *) malloc( sizeof(lc_char_t) * lc_string_size(p_string) );

	lc_strncpy( p_string->s, s, p_string->length + 1 );

	return p_string->s != NULL;
}

void lc_string_destroy( const lc_string_t* p_string )
{
	assert( p_string );
	free( p_string->s );
}

int lc_string_compare( const lc_string_t* left, const lc_string_t* right )
{
	assert( left && right );
	return lc_strncmp( left->s, right->s, left->length );
}

int lc_string_ncompare( const lc_string_t* left, const lc_string_t* right, size_t n )
{
	assert( left && right );
	return lc_strncmp( left->s, right->s, n );
}

int lc_string_casecompare( const lc_string_t* left, const lc_string_t* right )
{
	assert( left && right );
	#if !defined(wcsncasecmp) && defined(UNICODE)
	lc_char_t* tmp_left  = lc_strtolower( lc_strdup( left->s ) );
	lc_char_t* tmp_right = lc_strtolower( lc_strdup( right->s ) );
	int result = lc_strcmp( tmp_left, tmp_right );
	free( tmp_left );
	free( tmp_right );
	return result;
	#else
	return lc_strncasecmp( left->s, right->s, left->length );
	#endif
}

int lc_string_ncasecompare( const lc_string_t* left, const lc_string_t* right, size_t n )
{
	assert( left && right );
	#if !defined(wcsncasecmp) && defined(UNICODE)
	lc_char_t* tmp_left  = lc_strtolower( lc_strdup( left->s ) );
	lc_char_t* tmp_right = lc_strtolower( lc_strdup( right->s ) );
	int result = lc_strncmp( tmp_left, tmp_right, n );
	free( tmp_left );
	free( tmp_right );
	return result;
	#else
	return lc_strncasecmp( left->s, right->s, n );
	#endif
}

void lc_string_tolower( lc_string_t* p_string )
{
	lc_char_t *p_s;

	assert( p_string );
	p_s = p_string->s;

	while( p_s )
	{
		*p_s = lc_char_lower( *p_s );
		p_s++;
	}
}

void lc_string_toupper( lc_string_t* p_string )
{
	lc_char_t *p_s;

	assert( p_string );
	p_s = p_string->s;

	while( p_s )
	{
		*p_s = lc_char_upper( *p_s );
		p_s++;
	}
}

bool lc_string_copy( lc_string_t* p_result, const lc_string_t* p_string )
{
	assert( p_result && p_string );

	p_result->length = lc_string_length( p_string );
	p_result->s      = (lc_char_t *) realloc( p_result->s, lc_string_size( p_string ) );

	if( p_result->s )
	{
		lc_strncpy(
			lc_string_string( p_result ),
			lc_string_string( p_string ),
			lc_string_length( p_string ) + 1
		);
		return true;
	}

	return false;
}

bool lc_string_ncopy( lc_string_t* p_result, const lc_string_t* p_string, size_t n )
{
	assert( p_result && p_string );
	assert( n > 0 && n <= lc_string_length( p_string ) );

	p_result->length = lc_string_length( p_string );
	p_result->s      = (lc_char_t *) realloc( p_result->s, (1 + n) * sizeof(lc_char_t) );

	if( p_result->s )
	{
		lc_strncpy(
			lc_string_string( p_result ),
			lc_string_string( p_string ),
			n
		);
		return true;
	}

	return false;
}

bool lc_string_assign( lc_string_t* p_result, const lc_char_t *p_string )
{
	assert( p_result && p_string );

	p_result->length = lc_strlen( p_string );
	p_result->s      = (lc_char_t *) realloc( p_result->s, lc_string_length( p_result ) + 1 );

	if( p_result->s )
	{
		lc_strncpy(
			lc_string_string( p_result ),
			p_string,
			lc_string_length( p_result ) + 1
		);
		return true;
	}

	return false;
}

bool lc_string_concatenate( lc_string_t* p_result, const lc_string_t* p_string )
{
	size_t size;
	assert( p_result && p_string );

	size        = lc_string_length( p_result ) + lc_string_length( p_string ) + 1;
	p_result->s = (lc_char_t *) realloc( p_result->s, size );

	if( p_result->s )
	{
		lc_strncat(
			lc_string_string( p_result ),
			lc_string_string( p_string ),
			lc_string_length( p_string )
		);
		return true;
	}

	return false;
}

bool lc_string_nconcatenate( lc_string_t* p_result, const lc_string_t* p_string, size_t n )
{
	size_t size;
	assert( p_result && p_string );

	size        = lc_string_size( p_result ) + sizeof(lc_char_t) * (n + 1);
	p_result->s = (lc_char_t *) realloc( p_result->s, size );

	if( p_result->s )
	{
		lc_strncat( lc_string_string( p_result ), lc_string_string( p_string ), n );
		return true;
	}

	return false;
}

bool lc_string_sconcatenate( lc_string_t* p_string, const lc_char_t *s )
{
	size_t size;
	size_t len;

	assert( p_string && s );
	len = lc_strlen( s );

	size        = lc_string_size( p_string ) + sizeof(lc_char_t) * (len + 1);
	p_string->s = (lc_char_t *) realloc( p_string->s, size );

	if( p_string->s )
	{
		lc_strncat( lc_string_string(p_string), s, len );
		return true;
	}

	return false;
}

void lc_string_format( lc_string_t* p_string, const lc_char_t *format, ... )
{
	va_list args;
	size_t buffer_length;

	assert( p_string );
	assert( format );

	va_start( args, format );
	buffer_length = lc_vsnprintf( NULL, 0, format, args );
	va_end( args );

	if( buffer_length > 0 )
	{
		if( lc_string_size(p_string) < buffer_length )
		{
			free( p_string->s );
			p_string->length = buffer_length;
			p_string->s      = (lc_char_t *) malloc( sizeof(lc_char_t) * lc_string_size(p_string) );
		}

		va_start( args, format );
		lc_vsnprintf( p_string->s, buffer_length, format, args );
		p_string->s[ buffer_length ] = '\0';
		va_end( args );
	}
}

size_t lc_string_ltrim( lc_string_t* p_string )
{
	size_t charsRemoved = 0;
	assert( p_string );
	charsRemoved  = lc_ltrim( p_string->s, _T("\n\r\t ") );
	p_string->length -= charsRemoved;
	return charsRemoved;
}

size_t lc_string_rtrim( lc_string_t* p_string )
{
	size_t charsRemoved = 0;
	assert( p_string );
	charsRemoved = lc_rtrim( p_string->s, _T("\n\r\t ") );
	p_string->length -= charsRemoved;
	return charsRemoved;
}

size_t lc_string_trim( lc_string_t* p_string )
{
	return lc_string_rtrim( p_string ) + lc_string_ltrim( p_string );
}

size_t lc_ltrim( lc_char_t* s, const lc_char_t* delimeters )
{
	lc_char_t* start = s;
	assert( s );

	while( *s && lc_strchr(delimeters, *s) )
	{
		++s;
	}

	lc_strcpy( start, s );
	return s - start; /* # of chars removed */
}

size_t lc_rtrim( lc_char_t* s, const lc_char_t* delimeters )
{
	lc_char_t *end = s + lc_strlen(s) - 1;
	lc_char_t *new_end = end;
	assert( s );

	if( s != NULL )
	{
		while( lc_strchr(delimeters, *new_end) && new_end >= s )
		{
			--new_end;
		}
		*(new_end + 1) = '\0';
	}

	return end - new_end;
}

size_t lc_trim( lc_char_t* s, const lc_char_t* delimeters )
{
	return lc_ltrim( s, delimeters ) + lc_rtrim( s, delimeters );
}

lc_char_t* lc_strtolower( lc_char_t* s )
{
	lc_char_t* p_s = s;
	while( p_s )
	{
		*p_s = lc_char_lower( *p_s );
		p_s++;
	}

	return s;
}

lc_char_t* lc_strtoupper( lc_char_t* s )
{
	lc_char_t* p_s = s;

	while( p_s )
	{
		*p_s = lc_char_upper( *p_s );
		p_s++;
	}

	return s;
}

#ifndef wcsdup
wchar_t* wcsdup( const wchar_t* s )
{
	size_t size = wcslen(s) + 1;
	wchar_t* result = (wchar_t*) malloc( sizeof(wchar_t) * size );
	memcpy( result, s, size );
	return result;
}
#endif
