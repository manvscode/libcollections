/*
 * Copyright (C) 2010 by Joseph A. Marrero.  http://www.manvscode.com/
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
#ifndef _BINARY_HEAP_H_
#define _BINARY_HEAP_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <assert.h>
#include "heap.h"
#include "vector.h"

#define lc_binary_heap_create(array, initial_size)           lc_vector_create(array, initial_size)
#define lc_binary_heap_destroy(array)                        lc_vector_destroy(array)
#define lc_binary_heap_size( array )                         lc_vector_size( array )
#define lc_binary_heap_clear( array )                        lc_vector_clear( array )
#define lc_binary_heap_reheapify( array, type, compare )     lc_heap_make( array, type, compare )
#define lc_binary_heap_peek( array )                         ((array)[ 0 ])
#define lc_binary_heap_push( array, data, type, compare )    lc_vector_push( array, data ); lc_heap_push( array, type, compare )
#define lc_binary_heap_pop( array, type, compare ) \
{ \
	assert( array ); \
    \
	if( lc_vector_size(array) > 1 ) \
	{ \
		type last_elem                  = array[ lc_vector_size(array) - 1 ]; \
		array[ lc_vector_size(array) - 1 ] = array[ 0 ]; \
		array[ 0 ]                      = last_elem; \
	} \
    \
	lc_vector_pop( array ); \
	lc_heap_pop( array, type, compare ); \
}



#ifdef __cplusplus
}
#endif
#endif /* _BINARY_HEAP_H_ */
