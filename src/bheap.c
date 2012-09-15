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

static __inline void bheap_heapify_up   ( bheap_t* p_bheap );
static __inline void bheap_heapify_down ( bheap_t* p_bheap );

boolean bheap_create( bheap_t* p_bheap, size_t element_size, size_t size, bheap_compare_function compare_callback, bheap_element_function destroy_callback )
{
	boolean result = FALSE;
	assert( p_bheap );

	p_bheap->compare = compare_callback;
	p_bheap->tmp     = malloc( element_size );

	if( p_bheap->tmp )
	{
		result = vector_create( &p_bheap->heap, element_size, size, destroy_callback );
	}

	return result;
}

void bheap_destroy( bheap_t* p_bheap )
{
	assert( p_bheap );
	free( p_bheap->tmp );
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
	bheap_heapify_up( p_bheap );

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
	bheap_heapify_down( p_bheap );

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

void bheap_heapify_up( bheap_t* p_bheap )
{
	boolean done = FALSE;
	size_t index = vector_size( &p_bheap->heap ) - 1;

	while( !done )
	{
		size_t parent_index = parent_of( index );

		assert( index >= 0 );
		assert( parent_index >= 0 );
		
		void* parent  = vector_get( &p_bheap->heap, parent_index );
		void* element = vector_get( &p_bheap->heap, index );

		if( p_bheap->compare( parent, element ) < 0 )
		{
			memcpy( p_bheap->tmp, parent, vector_element_size(&p_bheap->heap) );
			memcpy( parent, element, vector_element_size(&p_bheap->heap) );
			memcpy( element, p_bheap->tmp, vector_element_size(&p_bheap->heap) );

			index = parent_index;
		}
		else
		{
			done = TRUE;
		}			
	}
}

void bheap_heapify_down( bheap_t* p_bheap )
{
	boolean done = FALSE;
	size_t index = 0;
	size_t size  = vector_size( &p_bheap->heap );

	while( !done && index < size )
	{
		size_t left_index  = left_child_of( index );
		size_t right_index = right_child_of( index );
		size_t optimal_idx = index;

		if( left_index < size && p_bheap->compare( vector_get(&p_bheap->heap, optimal_idx), vector_get(&p_bheap->heap, left_index) ) < 0 )
		{
			optimal_idx = left_index;
		}
		if( right_index < size && p_bheap->compare( vector_get(&p_bheap->heap, optimal_idx), vector_get(&p_bheap->heap, right_index) ) < 0 )
		{
			optimal_idx = right_index;
		}

		if( optimal_idx != index )
		{
			void* parent = vector_get( &p_bheap->heap, index );

			memcpy( p_bheap->tmp, parent, vector_element_size(&p_bheap->heap) );
			memcpy( parent, vector_get(&p_bheap->heap, optimal_idx), vector_element_size(&p_bheap->heap) );
			memcpy( vector_get(&p_bheap->heap, optimal_idx), p_bheap->tmp, vector_element_size(&p_bheap->heap) );

			index = optimal_idx;
		}			
		else
		{
			done = TRUE;
		}
	}
}

