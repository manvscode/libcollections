/*
 * Copyright (C) 2010-2014 by Joseph A. Marrero.  http://www.manvscode.com/
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
#include <assert.h>
#ifdef DEBUG_VECTOR
#include <stdio.h>
#endif
#include "vector.h"

void* __vector_resize( void* array, size_t element_size, size_t capacity )
{
	assert( element_size > 0 );

	if( vector_size(array) > capacity )
	{
		#if 0
		while( vector_size(array) > capacity )
		{
			vector_pop( array );
		}
		#else
		vector_s( array ) = capacity;
		#endif

	}

	size_t *result = (size_t*) realloc( array ? vector_raw(array) : NULL, 2 * sizeof(size_t) + element_size * capacity);

	if( result )
	{
		if( !array )
		{
			result[ 0 ] = 0;
		}
#ifdef DEBUG_VECTOR
		printf( "vector(@%p) size = %ld\n", result, capacity );
#endif

		result[ 1 ] = capacity;
		return result + 2;
	}
	else
	{
		free( vector_raw(array) );
	}

	return NULL; /* Force a crash */
}

