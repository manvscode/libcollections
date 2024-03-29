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
#include <time.h>
#include <array.h>

#define MAX 20
/**
 * 	\example example-array.c
 * 	This is an example of how to use the array collection.
 */
int main( int argc, char *argv[] )
{
	/**
	 * The array collection's state is contained in a
	 * lc_array_t object.
	 */
	lc_array_t a;
	int i;

	/**
	 * This is how you create an array collection.
	 */
	lc_array_create( &a, sizeof(double), 1 /* initial size */, malloc, free );
	srand( time(NULL) );

	/**
	 * This is how you resize an array collection.
	 */
	lc_array_resize( &a, MAX );

	for( i = 0; i < MAX; i++ )
	{
		/**
		 * This is how you access elements in the array collection.
		 */
		double *p_num = lc_array_elem( &a, i, double );
		*p_num = ((double) rand( )) / RAND_MAX;
	}

	for( i = 0; i < MAX; i++ )
	{
		double *p_num = lc_array_elem( &a, i, double );

		printf( "array[%2d] = %f\n", i, *p_num );
	}

	/**
	 * This is how you destroy an array collection.
	 */
	lc_array_destroy( &a );

	return 0;
}
