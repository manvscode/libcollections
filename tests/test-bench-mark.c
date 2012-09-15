#include <stdio.h>
#include "bench-mark.h"

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
		bench_mark_t bm = bench_mark_create( "Using Function" );

		bench_mark_start( bm );
		printf( "[" );
		for( i = 0; i < MAX; i++ )
		{
			val = func_test( MAX - i, (i * 2 + 1) % MAX );
			printf( "%d ", val );
		}
		printf( "]\n" );
		bench_mark_end( bm );

		bench_mark_report( bm );
		bench_mark_destroy( bm );
	}

	{
		bench_mark_t bm = bench_mark_create( "Using Macro" );

		bench_mark_start( bm );
		printf( "[" );
		for( i = 0; i < MAX; i++ )
		{
			val = macro_test( MAX - i, (i * 2 + 1) % MAX );
			printf( "%d ", val );
		}
		printf( "]\n" );
		bench_mark_end( bm );

		bench_mark_report( bm );
		bench_mark_destroy( bm );
	}

	return 0;
}
