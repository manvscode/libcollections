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
#include <stdbool.h>
#include <string.h>
#include "buffer.h"

typedef unsigned char byte_t;

struct lc_buffer {
	size_t size;
	byte_t data[];
};

lc_buffer_t* lc_buffer_create( size_t size, bool zero )
{
	size_t lc_buffer_size = sizeof(lc_buffer_t) + size;
	lc_buffer_t* p_buffer = (lc_buffer_t*) malloc( lc_buffer_size );

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

void lc_buffer_destroy( lc_buffer_t** p_buffer )
{
	if( *p_buffer )
	{
		free( *p_buffer );
		*p_buffer = NULL;
	}
}

size_t lc_buffer_size( lc_buffer_t* p_buffer )
{
    return p_buffer->size;
}

void* lc_buffer_data( lc_buffer_t* p_buffer )
{
    return p_buffer->data;
}

bool lc_buffer_resize( lc_buffer_t** p_buffer, size_t size )
{
	size_t new_size = sizeof(lc_buffer_t) + size;
	lc_buffer_t* new_buffer = (lc_buffer_t*) realloc( *p_buffer, new_size );

	if( new_buffer )
	{
		new_buffer->size = size;
        *p_buffer = new_buffer;
	}

	return new_buffer != NULL;
}

bool lc_buffer_put( lc_buffer_t** p_buffer, size_t offset, const void* buffer, size_t size, bool resize )
{
    bool result = true;

    if( p_buffer && *p_buffer )
    {
        size_t final_size = offset + size;

        if( final_size > (*p_buffer)->size )
        {
            // Buffer is too small so let's try to
            // resize it.
            if( resize )
            {
                if( !lc_buffer_resize( p_buffer, final_size ) )
                {
                    // resize failed
                    result = false;
                    goto done;
                }
            }
            else
            {
                // Cannot resize
                result = false;
                goto done;
            }
        }

        memcpy( (*p_buffer)->data + offset, buffer, size );
    }
    else
    {
        // bad input
        result = false;
        goto done;
    }

done:
    return result;
}

