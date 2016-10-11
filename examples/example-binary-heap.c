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
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#define VECTOR_GROW_AMOUNT(array)     (1)
#include <binary-heap.h>

#define SIZE  		12

/*
#define MAX_HEAP
*/

static int  int_compare1( int** left, int** right );
static int  int_compare2( const int* left, const int* right );
static bool num_destroy( void *data );

int main( int argc, char *argv[] )
{
	int i;
	time_t t = time(NULL);
	srand( t );

#if 1
	int** heap;
	lc_binary_heap_create( heap, 1 );

	printf( "seed = %ld\n", t );

	for( i = 0; i < SIZE; i++ )
	{
		int *num = (int*) malloc( sizeof(int) );
		*num = rand() % 50;

		lc_binary_heap_push( heap, num, int*, int_compare1 );
	}

	for( i = 0; i < 0.25 * SIZE; i++ )
	{
		int *num = lc_binary_heap_peek( heap );
		num_destroy( num );

		lc_binary_heap_pop( heap, int*, int_compare1 );
	}

	for( i = 0; i < 0.4 * SIZE; i++ )
	{
		int *num = (int*) malloc( sizeof(int) );
		*num = rand() % 200;
		lc_binary_heap_push( heap, num, int*, int_compare1 );
	}

	for( i = 0; i < 0.25 * SIZE; i++ )
	{
		int *num = lc_binary_heap_peek( heap );
		num_destroy( num );

		lc_binary_heap_pop( heap, int*, int_compare1 );
	}

	for( i = 0; i < 10; i++ )
	{
		int *num = (int*) malloc( sizeof(int) );
		*num = rand() % 15;
		lc_binary_heap_push( heap, num, int*, int_compare1 );
	}

	printf( "   ----- Sorted Output -----\n" );
	while( lc_binary_heap_size(heap) > 0 )
	{
		int* p_num = lc_binary_heap_peek( heap );
		printf( "%10d (size = %02ld) \n", *p_num, lc_binary_heap_size(heap) );

		num_destroy( p_num );

		lc_binary_heap_pop( heap, int*, int_compare1 );
	}

	lc_binary_heap_destroy( heap );
#else
	int* collection;
	vector_create( collection, 1 );

	for( i = 0; i < SIZE; i++ )
	{
		int num = (rand() % 130) + (rand() % 100)* pow(-1.0, i);
		vector_push( collection, num );
		heap_push( collection, int, int_compare2 )
	}

	printf( "unsorted = {" );
	for( i = 0; i < lc_vector_size(collection); i++ )
	{
		int num = collection[ i ];
		printf( "%4d%s", num, i < (lc_vector_size(collection) - 1) ? ", " : "" );
	}
	printf( "}\n" );

	//heap_make( collection, int, int_compare2 );

	printf( "  sorted = {" );
	while( lc_vector_size(collection) > 0 )
	{
		int num = collection[ 0 ];
		printf( "%4d%s", num, lc_vector_size(collection) > 1 ? ", " : "" );

		if( lc_vector_size(collection) > 1 )
		{
			int last_elem  = collection[ lc_vector_size(collection) - 1 ];

			collection[ lc_vector_size(collection) - 1 ] = collection[ 0 ];
			collection[ 0 ] = last_elem;
		}

		//num_destroy( p_num );
		vector_pop( collection );
		heap_pop( collection, int, int_compare2 );
	}
	printf( "}\n" );

	vector_destroy( collection );
#endif
	return 0;
}


int int_compare1( int** left, int** right )
{
	const int* p_left  = *left;
	const int* p_right = *right;

	#ifdef MAX_HEAP
	return (*p_left) - (*p_right); // max-heap
	#else
	return (*p_right) - (*p_left); // min-heap
	#endif
}

int int_compare2( const int* left, const int* right )
{
	const int* p_left  = left;
	const int* p_right = right;

	#ifdef MAX_HEAP
	return (*p_left) - (*p_right); // max-heap
	#else
	return (*p_right) - (*p_left); // min-heap
	#endif
}

bool num_destroy( void *data )
{
	free( data );
	return true;
}
