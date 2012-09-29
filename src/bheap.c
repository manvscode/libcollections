/*
 * Copyright (C) 2010 by Joseph A. Marrero and Shrewd LLC. http://www.manvscode.com/
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
#include <string.h>
#include <assert.h>
#include "vector.h"
#include "bheap.h"

#define parent_of( index )         ((index) >> 1)       /* index / 2 */
#define left_child_of( index )     (((index) << 1) + 0) /* 2 * index */
#define right_child_of( index )    (((index) << 1) + 1) /* 2 * index + 1 */

static void heapify( vector_t* heap, heap_compare_function compare, byte* swap_buffer, size_t index );

#ifdef USE_ALLOCATORS
boolean bheap_create( bheap_t* p_bheap, size_t element_size, size_t size, 
                      heap_compare_function compare_callback, heap_element_function destroy_callback,
                      alloc_function alloc, free_function free )
#else
boolean bheap_create( bheap_t* p_bheap, size_t element_size, size_t size, 
                      heap_compare_function compare_callback, heap_element_function destroy_callback )
#endif
{
	boolean result = FALSE;
	assert( p_bheap );

	#ifdef USE_ALLOCATORS
	p_bheap->alloc   = alloc;
	p_bheap->free    = free;
	p_bheap->compare = compare_callback;
	p_bheap->tmp     = p_bheap->alloc( element_size );
	#else
	p_bheap->compare = compare_callback;
	p_bheap->tmp     = malloc( element_size );
	#endif

	if( p_bheap->tmp )
	{
		#ifdef USE_ALLOCATORS
		result = vector_create( &p_bheap->heap, element_size, size, destroy_callback, p_bheap->alloc, p_bheap->free );
		#else
		result = vector_create( &p_bheap->heap, element_size, size, destroy_callback );
		#endif
	}

	return result;
}

void bheap_destroy( bheap_t* p_bheap )
{
	assert( p_bheap );
	#ifdef USE_ALLOCATORS
	p_bheap->free( p_bheap->tmp );
	#else
	free( p_bheap->tmp );
	#endif
	vector_destroy( &p_bheap->heap );
}

void* bheap_peek( bheap_t* p_bheap )
{
	assert( p_bheap );
	return vector_get( &p_bheap->heap, 0 );
}

boolean bheap_push( bheap_t* p_bheap, void* data )
{
	boolean result = FALSE;

	assert( p_bheap );
	result = vector_push( &p_bheap->heap, data );
	heap_push( &p_bheap->heap, p_bheap->compare, p_bheap->tmp );

	return result;
}

boolean bheap_pop( bheap_t* p_bheap )
{
	assert( p_bheap );
	boolean result = FALSE;

	memcpy( vector_get( &p_bheap->heap, 0 ), 
	        vector_get( &p_bheap->heap, vector_size(&p_bheap->heap) - 1 ), 
			vector_element_size( &p_bheap->heap ) 
	);

	result = vector_pop( &p_bheap->heap );
	heap_pop( &p_bheap->heap, p_bheap->compare, p_bheap->tmp );

	return result;	
}

size_t bheap_size( const bheap_t* p_bheap )
{
	return vector_size( &p_bheap->heap );
}

void bheap_clear( bheap_t* p_bheap )
{
	assert( p_bheap );
	vector_clear( &p_bheap->heap );
}

void heap_make( vector_t* heap, heap_compare_function compare, byte* swap_buffer )
{
}

void heap_push( vector_t* heap, heap_compare_function compare, byte* swap_buffer )
{
	boolean done = FALSE;
	size_t index = vector_size( heap ) - 1;

	while( !done )
	{
		size_t parent_index = parent_of( index );

		assert( index >= 0 );
		assert( parent_index >= 0 );
		
		void* parent  = vector_get( heap, parent_index );
		void* element = vector_get( heap, index );

		if( compare( parent, element ) < 0 )
		{
			memcpy( swap_buffer, parent, vector_element_size(heap) );
			memcpy( parent, element, vector_element_size(heap) );
			memcpy( element, swap_buffer, vector_element_size(heap) );

			index = parent_index;
		}
		else
		{
			done = TRUE;
		}			
	}
}

void heap_pop( vector_t* heap, heap_compare_function compare, byte* swap_buffer )
{
	heapify( heap, compare, swap_buffer, 0 );
}

void heapify( vector_t* heap, heap_compare_function compare, byte* swap_buffer, size_t index )
{
	boolean done = FALSE;
	//size_t index = 0;
	size_t size  = vector_size( heap );

	while( !done && index < size )
	{
		size_t left_index  = left_child_of( index );
		size_t right_index = right_child_of( index );
		size_t optimal_idx = index;

		if( left_index < size && compare( vector_get(heap, optimal_idx), vector_get(heap, left_index) ) < 0 )
		{
			optimal_idx = left_index;
		}
		if( right_index < size && compare( vector_get(heap, optimal_idx), vector_get(heap, right_index) ) < 0 )
		{
			optimal_idx = right_index;
		}

		if( optimal_idx != index )
		{
			void* parent = vector_get( heap, index );

			memcpy( swap_buffer, parent, vector_element_size(heap) );
			memcpy( parent, vector_get(heap, optimal_idx), vector_element_size(heap) );
			memcpy( vector_get(heap, optimal_idx), swap_buffer, vector_element_size(heap) );

			index = optimal_idx;
		}			
		else
		{
			done = TRUE;
		}
	}
}

