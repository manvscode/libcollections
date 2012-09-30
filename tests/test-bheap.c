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
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <bheap.h>

#define SIZE  		12

//#define MAX_HEAP	

static int     int_compare( const void* left, const void* right );
static boolean heap_elem_destroy( void *data );


int main( int argc, char *argv[] )
{
	time_t t = time(NULL);
	srand( t );
#if 0
	bheap_t heap;
	int i;

	bheap_create( &heap, sizeof(int), 1, int_compare, heap_elem_destroy, malloc, free );

	printf( "seed = %ld\n", t );

	for( i = 0; i < SIZE; i++ )
	{
		int num = (rand() % 130) + (rand() % 100)* pow(-1.0, i);
		bheap_push( &heap, &num );
	}
	
	for( i = 0; i < 0.25 * SIZE; i++ )
	{
		bheap_pop( &heap );
	}
	
	for( i = 0; i < SIZE; i++ )
	{
		int num = (rand() % 130) + (rand() % 100)* pow(-1.0, i);
		bheap_push( &heap, &num );
	}
	
	for( i = 0; i < 0.25 * SIZE; i++ )
	{
		bheap_pop( &heap );
	}

	printf( "   ----- Sorted Output -----\n" );
	while( bheap_size(&heap) > 0 )
	{
		int* p_num = bheap_peek( &heap );
		printf( "%10d (size = %02ld) \n", *p_num, bheap_size(&heap) );

		bheap_pop( &heap );
	}

	bheap_destroy( &heap );
#else
	int i;
	vector_t collection;
	vector_create( &collection, sizeof(int), 1, heap_elem_destroy, malloc, free );

	for( i = 0; i < SIZE; i++ )
	{
		int num = (rand() % 130) + (rand() % 100)* pow(-1.0, i);
		vector_push( &collection, &num );
	}

	printf( "unsorted = {" );
	for( i = 0; i < vector_size(&collection); i++ )
	{
		int* p_num = vector_get( &collection, i );
		printf( "%4d%s", *p_num, i < vector_size(&collection) - 1 ? ", " : "" );
	}
	printf( "}\n" );

	int tmp = 0;
	heap_make( &collection, int_compare, (byte*) &tmp );

	printf( "  sorted = {" );
	while( vector_size(&collection) > 0 )
	{
		int* p_num = vector_get( &collection, 0 );
		printf( "%4d%s", *p_num, vector_size(&collection) > 1 ? ", " : "" );

		if( vector_size(&collection) > 1 )
		{
			memcpy( vector_get( &collection, 0 ), 
					vector_get( &collection, vector_size(&collection) - 1 ), 
					vector_element_size( &collection ) 
			);
		}
		vector_pop( &collection );
		heap_pop( &collection, int_compare, (byte*) &tmp );
	}
	printf( "}\n" );

	vector_destroy( &collection );
#endif
	return 0;
}


int int_compare( const void* left, const void* right )
{
	const int* p_left = left;
	const int* p_right = right;

	#ifdef MAX_HEAP	
	return (*p_left) - (*p_right); // max-heap
	#else
	return (*p_right) - (*p_left); // min-heap
	#endif
}

boolean heap_elem_destroy( void *data )
{
	return TRUE;
}
