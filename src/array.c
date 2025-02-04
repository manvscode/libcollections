/*
 * Copyright (C) 2010-2025 by Joseph A. Marrero.  https://joemarrero.com/
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
#include <stdio.h>
#include <assert.h>
#include "array.h"
#ifdef _DEBUG_VECTOR
#include <string.h>
#endif

bool lc_array_create( lc_array_t* p_array, size_t element_size, size_t size, lc_alloc_fxn_t alloc, lc_free_fxn_t free )
{
	assert( p_array );

	p_array->element_size = element_size;
	p_array->size         = size;
	p_array->alloc        = alloc;
	p_array->free         = free;
	p_array->arr          = p_array->alloc( lc_array_element_size(p_array) * lc_array_size(p_array) );

	#ifdef _DEBUG_VECTOR
	memset( p_array->arr, 0, lc_array_element_size(p_array) * lc_array_size(p_array) );
	#endif

	assert( p_array->arr );

	return p_array->arr != NULL;
}

void lc_array_destroy( lc_array_t* p_array )
{
	assert( p_array );

	p_array->free( p_array->arr );

	#ifdef _DEBUG_VECTOR
	p_array->element_size = 0L;
	p_array->arr          = NULL;
	p_array->size         = 0L;
	#endif
}

bool lc_array_resize( lc_array_t* p_array, size_t new_size )
{
	bool result = true;

	if( lc_array_size(p_array) != new_size )
	{
		p_array->size  = new_size;
		p_array->arr   = realloc( p_array->arr, p_array->element_size * lc_array_size(p_array) );

		result = p_array->arr != NULL;
	}

	return result;
}

#if !defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L) /* Not C99 */
void* lc_array_element( lc_array_t* p_array, size_t index )
{
	assert( index >= 0 );
	assert( index < lc_array_size(p_array) );
	return (void*)(lc_array_base(p_array) + lc_array_element_size(p_array) * (index));
}
#endif

bool lc_array_serialize( lc_array_t* p_array, FILE* file, lc_array_serialize_fxn_t func )
{
	if( fwrite( &p_array->size, sizeof(size_t), 1, file ) == 1 )
	{
		if( !func )
		{
			if( fwrite( p_array->arr, p_array->element_size, p_array->size, file ) == p_array->size )
			{
				return true;
			}
		}
		else
		{
			/* User passed serialization function */
			return func( p_array->arr );
		}
	}

	return false;
}

bool lc_array_unserialize( lc_array_t* p_array, FILE* file, lc_array_unserialize_fxn_t func )
{
	size_t new_size = 0;

	if( fread( &new_size, sizeof(size_t), 1, file ) == 1 )
	{
		lc_array_resize( p_array, new_size );

		if( !func )
		{
			if( fread( p_array->arr, p_array->element_size, p_array->size, file ) == p_array->size )
			{
				return true;
			}
		}
		else
		{
			/* User passed unserialization function */
			return func( p_array->arr );
		}
	}

	return false;
}

