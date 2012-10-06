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
#ifndef _BIN_HEAP_H_
#define _BIN_HEAP_H_
#ifdef __cplusplus
extern "C" {
#endif 
#include "types.h"
#include "vector.h"
#include "alloc.h"
#define parent_of( index )         ((index) >> 1)       /* index / 2 */
#define left_child_of( index )     (((index) << 1) + 0) /* 2 * index */
#define right_child_of( index )    (((index) << 1) + 1) /* 2 * index + 1 */
#include "bheap-template.h"

typedef boolean (*heap_serialize_function)   ( void *p_array );
typedef boolean (*heap_unserialize_function) ( void *p_array );
typedef boolean (*heap_element_function)     ( void *data );
typedef int     (*heap_compare_function)     ( const void *p_data_left, const void *p_data_right );


typedef struct bheap {
	alloc_function  alloc;
	free_function   free;

	heap_compare_function compare;
	vector_t  heap;
	byte*     tmp;
} bheap_t;


boolean         bheap_create      ( bheap_t* p_bheap, size_t element_size, size_t size, 
                                    heap_compare_function compare_callback, heap_element_function destroy_callback,
                                    alloc_function alloc, free_function free );
void            bheap_destroy     ( bheap_t* p_bheap );
void*           bheap_peek        ( bheap_t* p_bheap );
boolean         bheap_push        ( bheap_t* p_bheap, void* data );
boolean         bheap_pop         ( bheap_t* p_bheap );
size_t          bheap_size        ( const bheap_t* p_bheap );
void            bheap_clear       ( bheap_t* p_bheap );
void            bheap_reheapify   ( bheap_t* p_bheap );


void            heap_make         ( vector_t* heap, heap_compare_function compare, void* swap_buffer );
void            heap_push         ( vector_t* heap, heap_compare_function compare, void* swap_buffer );
void            heap_pop          ( vector_t* heap, heap_compare_function compare, void* swap_buffer );


/*
 *  Pointer Binary Heap
 */
typedef struct pbheap {
	heap_compare_function compare;
	pvector_t  heap;
} pbheap_t;

boolean         pbheap_create      ( pbheap_t* p_bheap, size_t size, heap_compare_function compare_callback,
                                     alloc_function alloc, free_function free );
void            pbheap_destroy     ( pbheap_t* p_bheap );
void            pbheap_clear       ( pbheap_t* p_bheap );
void            pbheap_reheapify   ( pbheap_t* p_bheap );

static inline void pheapify( pvector_t* heap, heap_compare_function compare, size_t index )
{
	boolean done = FALSE;
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
			void* parent  = pvector_get( heap, index );
			void* optimal = pvector_get( heap, optimal_idx );

			void* tmp = parent;
			pvector_set( heap, index, optimal );
			pvector_set( heap, optimal_idx, tmp );

			index = optimal_idx;
		}			
		else
		{
			done = TRUE;
		}
	}
}

static inline void pheap_make( pvector_t* heap, heap_compare_function compare )
{
	long index = parent_of( pvector_size( heap ) - 1 );

	while( index >= 0 )
	{
		pheapify( heap, compare, index );
		index--;
	}
}

static inline void pheap_push( pvector_t* heap, heap_compare_function compare )
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

static inline void pheap_pop( pvector_t* heap, heap_compare_function compare )
{
	pheapify( heap, compare, 0 );
}
	
static inline size_t pbheap_size( const pbheap_t* p_bheap )
{
	return pvector_size( &p_bheap->heap );
}

static inline void* pbheap_peek( pbheap_t* p_bheap )
{
	assert( p_bheap );
	return pvector_get( &p_bheap->heap, 0 );
}

static inline boolean pbheap_push( pbheap_t* p_bheap, void* data )
{
	boolean result = FALSE;

	assert( p_bheap );
	result = pvector_push( &p_bheap->heap, data );
	pheap_push( &p_bheap->heap, p_bheap->compare );

	return result;
}


static inline boolean pbheap_pop( pbheap_t* p_bheap )
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

#ifdef __cplusplus
}
#endif 
#endif /* _BIN_HEAP_H_ */
