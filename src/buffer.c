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
#include <stdbool.h>
#include <string.h>
#include "buffer.h"

struct buffer {
	size_t size;
	uint8_t data[];
};

buffer_t* buffer_create( size_t size, bool zero )
{
	size_t buffer_size = sizeof(buffer_t) + size;
	buffer_t* p_buffer = (buffer_t*) malloc( buffer_size );

	if( p_buffer )
	{
		p_buffer->size = size;

		if( zero )
		{
			memset( p_buffer->data, 0, size );
		}
	}

	return p_buffer;
}

void buffer_destroy( buffer_t** p_buffer )
{
	if( *p_buffer )
	{
		free( *p_buffer );
		*p_buffer = NULL;
	}
}

bool buffer_resize( buffer_t** p_buffer, size_t size )
{
	size_t new_size = sizeof(buffer_t) + size;
	buffer_t* new_buffer = (buffer_t*) realloc( *p_buffer, new_size );

	if( new_buffer )
	{
		new_buffer->size = size;
        *p_buffer = new_buffer;
	}

	return new_buffer != NULL;
}

size_t buffer_size( buffer_t* p_buffer )
{
    return p_buffer->size;
}

uint8_t* buffer_data( buffer_t* p_buffer )
{
    return p_buffer->data;
}
