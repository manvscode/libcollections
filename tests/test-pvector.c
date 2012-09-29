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
#include "vector.h"

typedef struct point {
	double x;
	double y;
	char name;
} point_t;

boolean point_destroy( void *data )
{
	/* vector makes a shallow copy of the
 	 * data on the array. There is nothing
 	 * to free.
 	 */
	free( data );
	return TRUE;
}

#define SIZE  26

#define create_point( )  ((point_t *) malloc( sizeof(point_t) ))


int main( int argc, char *argv[] )
{
	pvector_t collection;
	int i;
	char names[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	srand( 0 );

	pvector_create( &collection, 1, point_destroy, malloc, free );

	for( i = 0; i < SIZE; i++ )
	{
		point_t *p_pt = create_point( );

		p_pt->x    = ((rand( ) % 100) - 50.0);
		p_pt->y    = ((rand( ) % 100) - 50.0);
		p_pt->name = names[ i % (sizeof(names) - 1) ];

		pvector_push( &collection, p_pt );
	}

	for( i = 0; i < SIZE; i++ )
	{
		const point_t *p_pt = pvector_get( &collection, i );

		printf( "%c = (%3.0f, %3.0f)\n", 
			p_pt->name, 
			p_pt->x, 
			p_pt->y );
	}

	pvector_destroy( &collection );
	return 0;
}