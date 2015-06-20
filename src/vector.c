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

bool vector_reserve_capacity( void** array, size_t element_size, size_t capacity )
{
	bool result = false;

	assert( element_size > 0 );

	if( vector_size(*array) > capacity )
	{
		#if 0
		while( vector_size(*array) > capacity )
		{
			vector_pop( *array );
		}
		#else
		/* Set the new size which at a minimum is the capacity! */
		vector_s( *array ) = capacity;
		#endif

	}

	const size_t size = 2 * sizeof(size_t) + element_size * capacity;
	size_t *new_array = (size_t*) realloc( *array ? vector_raw(*array) : NULL, size );

	if( new_array )
	{
		if( !(*array) )
		{

			new_array[ 0 ] = 0;
		}
#ifdef DEBUG_VECTOR
		printf( "vector(@%p) capacity = %ld\n", new_array, capacity );
#endif

		new_array[ 1 ] = capacity;
		*array = new_array + 2;
		result = true;
	}
	else
	{
		/* Resizing failed, so we return the original vector */
		new_array = vector_raw(*array) + 2;
	}

	return result;
}

