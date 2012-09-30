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
#include <time.h>
#include "bheap.h"

#define SIZE  		20

//#define MAX_HEAP	

static int     int_compare( const void* left, const void* right );
static boolean heap_elem_destroy( void *data );


int main( int argc, char *argv[] )
{
	pbheap_t heap;
	int i;

	time_t t = time(NULL);

	srand( t );
	pbheap_create( &heap, 1, int_compare, heap_elem_destroy, malloc, free );

	printf( "seed = %ld\n", t );

	for( i = 0; i < SIZE; i++ )
	{
		int *num = (int*) malloc( sizeof(int) );
		*num = rand() % 50;
		pbheap_push( &heap, num );
	}

	for( i = 0; i < 0.25 * SIZE; i++ )
	{
		pbheap_pop( &heap );
	}
	
	for( i = 0; i < 0.4 * SIZE; i++ )
	{
		int *num = (int*) malloc( sizeof(int) );
		*num = rand() % 200;
		pbheap_push( &heap, num );
	}

	for( i = 0; i < 0.25 * SIZE; i++ )
	{
		pbheap_pop( &heap );
	}

	for( i = 0; i < 10; i++ )
	{
		int *num = (int*) malloc( sizeof(int) );
		*num = rand() % 15;
		pbheap_push( &heap, num );
	}

	printf( "   ----- Sorted Output -----\n" );
	while( pbheap_size(&heap) > 0 )
	{
		int* p_num = pbheap_peek( &heap );
		printf( "%10d (size = %02ld) \n", *p_num, pbheap_size(&heap) );

		pbheap_pop( &heap );
	}

	pbheap_destroy( &heap );
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
	free( data );
	return TRUE;
}