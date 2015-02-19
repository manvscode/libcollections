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
#ifndef _BINARY_HEAP_H_
#define _BINARY_HEAP_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <assert.h>
#include "heap.h"
#include "vector.h"



#define binary_heap_create(array, initial_size)           vector_create(array, initial_size)
#define binary_heap_destroy(array)                        vector_destroy(array)
#define binary_heap_size( array )                         vector_size( array )
#define binary_heap_clear( array )                        vector_clear( array )
#define binary_heap_reheapify( array, type, compare )     heap_make( array, type, compare )
#define binary_heap_peek( array )                         ((array)[ 0 ])
#define binary_heap_push( array, data, type, compare )    vector_push( array, data ); heap_push( array, type, compare )
#define binary_heap_pop( array, type, compare ) \
{ \
	assert( array ); \
    \
	if( vector_size(array) > 1 ) \
	{ \
		type last_elem                  = array[ vector_size(array) - 1 ]; \
		array[ vector_size(array) - 1 ] = array[ 0 ]; \
		array[ 0 ]                      = last_elem; \
	} \
    \
	vector_pop( array ); \
	heap_pop( array, type, compare ); \
}



#ifdef __cplusplus
}
#endif
#endif /* _BINARY_HEAP_H_ */
