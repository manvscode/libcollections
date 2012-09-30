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
#include <libcollections/vector.h>

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
	return TRUE;
}

#define SIZE  26

#define create_point( )  ((point_t *) malloc( sizeof(point_t) ))


int main( int argc, char *argv[] )
{
	vector_t collection;
	int i;
	char names[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	srand( time(NULL) );
	vector_create( &collection, sizeof(point_t), 1, point_destroy, malloc, free );

	for( i = 0; i < SIZE; i++ )
	{
		point_t pt;

		pt.x    = ((rand( ) % 100) - 50.0);
		pt.y    = ((rand( ) % 100) - 50.0);
		pt.name = names[ rand( ) % (sizeof(names) - 1) ];

		vector_push( &collection, &pt );
	}

	for( i = 0; i < 0.25 * SIZE; i++ )
	{
		vector_pop( &collection );
	}
	
	for( i = 0; i < 0.5 * SIZE; i++ )
	{
		point_t pt;

		pt.x    = ((rand( ) % 100) - 50.0);
		pt.y    = ((rand( ) % 100) - 50.0);
		pt.name = names[ rand( ) % (sizeof(names) - 1) ];

		vector_push( &collection, &pt );
	}

	for( i = 0; i < 0.25 * SIZE; i++ )
	{
		vector_pop( &collection );
	}
	
	for( i = 0; i < vector_size(&collection); i++ )
	{
		point_t *p_pt = (point_t *) vector_get( &collection, i );

		printf( "%c = (%03.2f, %03.2f)\n", 
			p_pt->name, 
			p_pt->x, 
			p_pt->y );
	}

	vector_destroy( &collection );
	return 0;
}
