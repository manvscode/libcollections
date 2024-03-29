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
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#define VECTOR_GROW_AMOUNT(array)     (1)
#include <vector.h>



typedef struct point {
	double x;
	double y;
	char name;
} point_t;

#define SIZE  26

#define create_point( )  ((point_t *) malloc( sizeof(point_t) ))

int main( int argc, char *argv[] )
{
	point_t* collection;

	int i;
	char names[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	srand( time(NULL) );
	lc_vector_create( collection, 1 );


	for( i = 0; i < SIZE; i++ )
	{
		point_t pt;

		pt.x    = ((rand( ) % 100) - 50.0);
		pt.y    = ((rand( ) % 100) - 50.0);
		pt.name = names[ rand( ) % (sizeof(names) - 1) ];
		pt.name = names[ rand( ) % (sizeof(names) - 1) ];
		pt.name = names[ rand( ) % (sizeof(names) - 1) ];

		bool success = lc_vector_push( collection, pt );

		if( !success )
		{
			printf( "push failed!\n" );
		}
	}

	for( i = 0; i < 10; i++ )
	{
		lc_vector_pop( collection );
	}

	for( i = 0; i < 15; i++ )
	{
		point_t pt;

		pt.x    = ((rand( ) % 100) - 50.0);
		pt.y    = ((rand( ) % 100) - 50.0);
		pt.name = names[ rand( ) % (sizeof(names) - 1) ];

		bool success = lc_vector_push( collection, pt );

		if( !success )
		{
			printf( "push failed!\n" );
		}
	}

	for( i = 0; i < 10; i++ )
	{
		lc_vector_pop( collection );
	}

	for( i = 0; i < lc_vector_size(collection); i++ )
	{

		point_t *p_pt = &collection[ i ];

		printf( "%c = (%03.2f, %03.2f)\n",
			p_pt->name,
			p_pt->x,
			p_pt->y );
	}

	printf( "Final Capacity = %zu\n", lc_vector_capacity(collection) );
	printf( "    Final Size = %zu\n", lc_vector_size(collection) );

	lc_vector_destroy( collection );
	return 0;
}
