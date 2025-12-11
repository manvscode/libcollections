/* Copyright (C) 2016-2025 by Joseph A. Marrero, http://www.joemarrero.com/
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
#include <stdint.h>
#include <stdarg.h>
#include "buffer.h"
#ifdef __linux__
# include <sys/types.h>
#endif
#include "textbuffer.h"


void lc_textbuffer_create( lc_textbuffer_t* textbuffer )
{
    textbuffer->buffer = lc_buffer_create( 512, false );
    textbuffer->count  = 0;
}

void lc_textbuffer_destroy( lc_textbuffer_t* textbuffer )
{
    if( textbuffer )
    {
        lc_buffer_destroy( &textbuffer->buffer );
    }
}

bool lc_textbuffer_printf( lc_textbuffer_t *p_buffer, const char *format, ... )
{
    bool result = false;
    va_list args;

    va_start( args, format );
    result = lc_textbuffer_vprintf( p_buffer, format, args );
    va_end( args );

    return result;
}

bool lc_textbuffer_vprintf( lc_textbuffer_t* p_buffer, const char *format, va_list args )
{
    bool result = false;
    va_list args_copy;
    va_copy(args_copy, args);

    const int GROWTH = 64;

retry:
    {
        ssize_t size_remaining = lc_buffer_size(p_buffer->buffer) - p_buffer->count;
        char* text = (char*) lc_buffer_data(p_buffer->buffer) + p_buffer->count;
        ssize_t ret = vsnprintf( text, size_remaining, format, args );
        text[ size_remaining - 1 ] = '\0';

        if( ret >= size_remaining )
        {
            // buffer is too small
            if( lc_buffer_resize( &p_buffer->buffer, lc_buffer_size(p_buffer->buffer) + ret + GROWTH + 1 ) )
            {
                va_end( args );
                va_copy( args, args_copy );
                goto retry;
            }
            else
            {
                fprintf( stderr, "ERROR: Unable to resize buffer.\n" );
                result = false;
            }
        }
        else if( ret > 0 )
        {
            p_buffer->count += ret;
        }
    }
    va_end( args_copy );

    return result;
}
