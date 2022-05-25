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
#ifndef _LC_HEAP_H_
#define _LC_HEAP_H_
/**
 * @file heap.h
 * @brief Related routines for maintaining heaps.
 *
 * @defgroup lc_heap Heap
 * @ingroup Collections
 *
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <assert.h>

#define lc_heap_parent_of( index )         ((index) >> 1)       /* index / 2 */
#define lc_heap_left_child_of( index )     (((index) << 1) + 0) /* 2 * index */
#define lc_heap_right_child_of( index )    (((index) << 1) + 1) /* 2 * index + 1 */


#define lc_heapify( array, type, compare, index ) \
{ \
	int done = 0; \
	size_t size  = lc_vector_size( array ); \
	size_t idx = index; \
    \
	while( !done && idx < size ) \
	{ \
		size_t left_index  = lc_heap_left_child_of( idx ); \
		size_t right_index = lc_heap_right_child_of( idx ); \
		size_t optimal_idx = idx; \
        \
		if( left_index < size && compare( &array[optimal_idx], &array[left_index] ) < 0 ) \
		{ \
			optimal_idx = left_index; \
		} \
		if( right_index < size && compare( &array[optimal_idx], &array[right_index] ) < 0 ) \
		{ \
			optimal_idx = right_index; \
		} \
        \
		if( optimal_idx != idx ) \
		{ \
			type tmp             = array[ idx ]; \
			array[ idx ]         = array[ optimal_idx ]; \
			array[ optimal_idx ] = tmp; \
			idx = optimal_idx; \
		} \
		else \
		{ \
			done = 1; \
		} \
	} \
}

#define lc_heap_make( array, type, compare ) \
{ \
	ssize_t index = lc_heap_parent_of( lc_vector_size( array ) - 1 ); \
	while( index >= 0 ) \
	{ \
		lc_heapify( array, type, compare, index ); \
		index--; \
	} \
}

#define lc_heap_push( array, type, compare ) \
{ \
	int done = 0; \
	size_t index = lc_vector_size( array ) - 1; \
    \
	while( !done ) \
	{ \
		size_t parent_index = lc_heap_parent_of( index ); \
        \
		assert( index >= 0 ); \
		assert( parent_index >= 0 ); \
        \
		if( compare( &array[ parent_index ], &array[ index ] ) < 0 ) \
		{ \
			type tmp              = array[ parent_index ]; \
			array[ parent_index ] = array[ index ]; \
			array[ index ]        = tmp; \
            \
			index = parent_index; \
		} \
		else \
		{ \
			done = 1; \
		} \
	} \
}

#define lc_heap_pop( array, type, compare )     lc_heapify( array, type, compare, 0 )


#ifdef __cplusplus
}
#endif
#endif /* _LC_HEAP_H_ */
