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

static void heapify  ( vector_t* heap, heap_compare_function compare, void* swap_buffer, size_t index );
static void pheapify( pvector_t* heap, heap_compare_function compare, size_t index );

boolean bheap_create( bheap_t* p_bheap, size_t element_size, size_t size, 
                      heap_compare_function compare_callback, heap_element_function destroy_callback,
                      alloc_function alloc, free_function free )
{
	boolean result = FALSE;
	assert( p_bheap );

	p_bheap->alloc   = alloc;
	p_bheap->free    = free;
	p_bheap->compare = compare_callback;
	p_bheap->tmp     = p_bheap->alloc( element_size );

	if( p_bheap->tmp )
	{
		result = vector_create( &p_bheap->heap, element_size, size, destroy_callback, p_bheap->alloc, p_bheap->free );
	}

	return result;
}

void bheap_destroy( bheap_t* p_bheap )
{
	assert( p_bheap );
	p_bheap->free( p_bheap->tmp );
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

	if( vector_size(&p_bheap->heap) > 1 )
	{
		memcpy( vector_get( &p_bheap->heap, 0 ), 
				vector_get( &p_bheap->heap, vector_size(&p_bheap->heap) - 1 ), 
				vector_element_size( &p_bheap->heap ) 
		);
	}

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

void bheap_reheapify( bheap_t* p_bheap )
{
	heap_make( &p_bheap->heap, p_bheap->compare, p_bheap->tmp );
}

void heap_make( vector_t* heap, heap_compare_function compare, void* swap_buffer )
{
	ssize_t index = parent_of( vector_size( heap ) - 1 );

	while( index >= 0 )
	{
		heapify( heap, compare, swap_buffer, index );
		index = parent_of( index );
	}
}

void heap_push( vector_t* heap, heap_compare_function compare, void* swap_buffer )
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

void heap_pop( vector_t* heap, heap_compare_function compare, void* swap_buffer )
{
	heapify( heap, compare, swap_buffer, 0 );
}

void heapify( vector_t* heap, heap_compare_function compare, void* swap_buffer, size_t index )
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




boolean pbheap_create( pbheap_t* p_bheap, size_t size, 
                       heap_compare_function compare_callback, heap_element_function destroy_callback,
                       alloc_function alloc, free_function free )
{
	boolean result = FALSE;
	assert( p_bheap );

	p_bheap->compare = compare_callback;
	result = pvector_create( &p_bheap->heap, size, destroy_callback, alloc, free );

	return result;
}

void pbheap_destroy( pbheap_t* p_bheap )
{
	assert( p_bheap );
	pvector_destroy( &p_bheap->heap );
}

void* pbheap_peek( pbheap_t* p_bheap )
{
	assert( p_bheap );
	return pvector_get( &p_bheap->heap, 0 );
}

boolean pbheap_push( pbheap_t* p_bheap, void* data )
{
	boolean result = FALSE;

	assert( p_bheap );
	result = pvector_push( &p_bheap->heap, data );
	pheap_push( &p_bheap->heap, p_bheap->compare );

	return result;
}

boolean pbheap_pop( pbheap_t* p_bheap )
{
	assert( p_bheap );
	boolean result = FALSE;
	             
	if( pvector_size(&p_bheap->heap) > 1 )
	{
		void* last_elem  = pvector_get( &p_bheap->heap, pvector_size(&p_bheap->heap) - 1 );
		void* first_elem = pvector_get( &p_bheap->heap, 0 );

		pvector_set( &p_bheap->heap, pvector_size(&p_bheap->heap) - 1, first_elem );
		pvector_set( &p_bheap->heap, 0, last_elem );
	}

	result = pvector_pop( &p_bheap->heap );
	pheap_pop( &p_bheap->heap, p_bheap->compare );

	return result;	
}

size_t pbheap_size( const pbheap_t* p_bheap )
{
	return pvector_size( &p_bheap->heap );
}

void pbheap_clear( pbheap_t* p_bheap )
{
	assert( p_bheap );
	pvector_clear( &p_bheap->heap );
}

void pbheap_reheapify( pbheap_t* p_bheap )
{
	pheap_make( &p_bheap->heap, p_bheap->compare );
}

void pheap_make( pvector_t* heap, heap_compare_function compare )
{
	size_t index = parent_of( pvector_size( heap ) - 1 );

	while( index >= 1 )
	{
		pheapify( heap, compare, index );
		index = parent_of( index );
	}
}

void pheap_push( pvector_t* heap, heap_compare_function compare )
{
	boolean done = FALSE;
	size_t index = pvector_size( heap ) - 1;

	while( !done )
	{
		size_t parent_index = parent_of( index );

		assert( index >= 0 );
		assert( parent_index >= 0 );
		
		void* parent  = pvector_get( heap, parent_index );
		void* element = pvector_get( heap, index );

		if( compare( parent, element ) < 0 )
		{
			void* tmp = parent;
			pvector_set( heap, parent_index, element );
			pvector_set( heap, index, tmp );

			index = parent_index;
		}
		else
		{
			done = TRUE;
		}			
	}
}

void pheap_pop( pvector_t* heap, heap_compare_function compare )
{
	pheapify( heap, compare, 0 );
}

void pheapify( pvector_t* heap, heap_compare_function compare, size_t index )
{
	boolean done = FALSE;
	//size_t index = 0;
	size_t size  = pvector_size( heap );

	while( !done && index < size )
	{
		size_t left_index  = left_child_of( index );
		size_t right_index = right_child_of( index );
		size_t optimal_idx = index;

		if( left_index < size && compare( pvector_get(heap, optimal_idx), pvector_get(heap, left_index) ) < 0 )
		{
			optimal_idx = left_index;
		}
		if( right_index < size && compare( pvector_get(heap, optimal_idx), pvector_get(heap, right_index) ) < 0 )
		{
			optimal_idx = right_index;
		}

		if( optimal_idx != index )
		{
			void* parent = pvector_get( heap, index );

			void* tmp = parent;
			pvector_set( heap, index, pvector_get(heap, optimal_idx) );
			pvector_set( heap, optimal_idx, tmp );

			index = optimal_idx;
		}			
		else
		{
			done = TRUE;
		}
	}
}
