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
#ifndef _BUFFER_H_
#define _BUFFER_H_
#include <stdbool.h>
#include <stdint.h>

struct buffer;
typedef struct buffer buffer_t;

buffer_t* buffer_create  ( size_t size, bool zero );
void      buffer_destroy ( buffer_t** p_buffer );
size_t    buffer_size    ( buffer_t* p_buffer );
void*     buffer_data    ( buffer_t* p_buffer );
bool      buffer_resize  ( buffer_t** p_buffer, size_t new_size );
bool      buffer_put     ( buffer_t** p_buffer, size_t offset, const void* buffer, size_t size, bool resize );

#endif /* _BUFFER_H_ */
