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
#include <stdio.h>
#include <benchmark.h>

#define MAX 200
#define macro_test(a, b) ((a)+(b))

int func_test( int a, int b )
{
	return a + b;
}

int main(void)
{
	unsigned int i;
	int val;

	{
		lc_benchmark_t bm = lc_benchmark_create( "Using Function" );

		lc_benchmark_start( bm );
		printf( "[" );
		for( i = 0; i < MAX; i++ )
		{
			val = func_test( MAX - i, (i * 2 + 1) % MAX );
			printf( "%d ", val );
		}
		printf( "]\n" );
		lc_benchmark_end( bm );

		lc_benchmark_report( bm );
		lc_benchmark_destroy( bm );
	}

	{
		lc_benchmark_t bm = lc_benchmark_create( "Using Macro" );

		lc_benchmark_start( bm );
		printf( "[" );
		for( i = 0; i < MAX; i++ )
		{
			val = macro_test( MAX - i, (i * 2 + 1) % MAX );
			printf( "%d ", val );
		}
		printf( "]\n" );
		lc_benchmark_end( bm );

		lc_benchmark_report( bm );
		lc_benchmark_destroy( bm );
	}

	return 0;
}
