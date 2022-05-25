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
#if defined(WIN32) || defined(WIN64)
#include <omp.h>
#else
#include <sys/time.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "benchmark.h"

static inline char* desc_dup( const char* s )
{
	size_t len = strlen( s );
	char* result = malloc( len + 1 );

	if( result )
	{
		memcpy( result, s, len + 1 );
	}

	return result;
}


struct lc_benchmark {

	#if defined(WIN32) || defined(WIN64)
	double start;
	double end;
	#else
	struct timeval start;
	struct timeval end;
	#endif
	const char *description;
};

lc_benchmark_t lc_benchmark_create( const char *description )
{
	lc_benchmark_t bm = (lc_benchmark_t) malloc( sizeof(struct lc_benchmark) );

	if( !bm )
	{
		goto failure;
	}

	bm->description = desc_dup( description );

	if( !bm->description )
	{
		goto failure;
	}

	return bm;

failure:
	if( bm && bm->description ) free( (void*) bm->description );
	if( bm ) free( bm );
	return NULL;
}

void lc_benchmark_destroy( lc_benchmark_t bm )
{
	assert( bm );
	free( (void *) bm->description );
	free( bm );
}

void lc_benchmark_start( lc_benchmark_t bm )
{
	assert( bm );
	#if defined(WIN32) || defined(WIN64)
	bm->start = omp_get_wtime();
	#else
	gettimeofday( &bm->start, NULL );
	#endif
}

void lc_benchmark_end( lc_benchmark_t bm )
{
	assert( bm );
	#if defined(WIN32) || defined(WIN64)
	bm->end = omp_get_wtime();
	#else
	gettimeofday( &bm->end, NULL );
	#endif
}

void lc_benchmark_report( lc_benchmark_t bm )
{
	#if defined(WIN32) || defined(WIN64)
	printf( "%30s -- Start: %.6lf s, End: %.6lf s, Total: %.3lf ms \n",
		bm->description,
		bm->start,
		bm->end,
		(bm->end - bm->start) * 1000.0
	);
	#else
	/*printf( "%30s -- Start: %ld.%06ld s, End: %ld.%06ld s, Total: %lf ms \n", */
	printf( "%30s -- Start: %ld.%06ld s, End: %ld.%06ld s, Total: %f ms \n",
		bm->description,
		bm->start.tv_sec, (long int) bm->start.tv_usec,
		bm->end.tv_sec, (long int) bm->end.tv_usec,
		1000.0 * (bm->end.tv_sec - bm->start.tv_sec) + ((bm->end.tv_usec - bm->start.tv_usec) / 1000.0)
	);
	#endif
}

double lc_benchmark_start_time( lc_benchmark_t bm )
{
	#if defined(WIN32) || defined(WIN64)
	return bm->start;
	#else
	return 1000.0 * bm->start.tv_sec + (bm->start.tv_usec / 1000.0);
	#endif
}

double lc_benchmark_end_time( lc_benchmark_t bm )
{
	#if defined(WIN32) || defined(WIN64)
	return bm->end;
	#else
	return 1000.0 * bm->end.tv_sec + (bm->end.tv_usec / 1000.0);
	#endif
}

double lc_benchmark_total_time( lc_benchmark_t bm )
{
	#if defined(WIN32) || defined(WIN64)
	return (bm->end - bm->start) * 1000.0;
	#else
	return 1000.0 * (bm->end.tv_sec - bm->start.tv_sec) + ((bm->end.tv_usec - bm->start.tv_usec) / 1000.0);
	#endif
}
