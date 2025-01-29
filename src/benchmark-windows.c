/*
 * Copyright (C) 2010-2025 by Joseph A. Marrero.  https://joemarrero.com/
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
#ifdef USE_OPENMP
# include <omp.h> // OpenMP
#else
# include <windows.h>
#endif
#include <stdint.h>
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
#ifndef USE_OPENMP
    int64_t ticks_per_second;
#endif
	double start;
	double end;
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

#ifndef USE_OPENMP
    LARGE_INTEGER ticks_per_second;
    // On systems that run Windows XP or later, the function will always
    // succeed and will thus never return zero.
    if( QueryPerformanceFrequency( &ticks_per_second) ) {
        bm->ticks_per_second = ticks_per_second.QuadPart;
    }
    else
    {
		goto failure;
    }
#endif

	return bm;

failure:
	if( bm && bm->description ) free( (void*) bm->description );
	if( bm ) free( bm );
	return NULL;
}

#ifndef USE_OPENMP
static inline int64_t get_ticks() {
    LARGE_INTEGER ticks;
    // On systems that run Windows XP or later the function
    // will always succeed and will thus never return zero.
    QueryPerformanceCounter( (LARGE_INTEGER *) &ticks );
    return ticks.QuadPart;
}
#endif

void lc_benchmark_destroy( lc_benchmark_t bm )
{
	assert( bm );
	free( (void *) bm->description );
	free( bm );
}

void lc_benchmark_start( lc_benchmark_t bm )
{
	assert( bm );
#ifdef USE_OPENMP
	bm->start = omp_get_wtime();
#else
    UINT64 ticks = get_ticks();
    bm->start = (double) ticks / (double)bm->ticks_per_second;
#endif
}

void lc_benchmark_end( lc_benchmark_t bm )
{
	assert( bm );
#ifdef USE_OPENMP
	bm->end = omp_get_wtime();
#else
    UINT64 ticks = get_ticks();
    bm->end = (double) ticks / (double)bm->ticks_per_second;
#endif
}

void lc_benchmark_report( lc_benchmark_t bm )
{
	printf( "%30s -- Start: %.6lf s, End: %.6lf s, Total: %.3lf ms \n",
		bm->description,
		bm->start,
		bm->end,
		(bm->end - bm->start) * 1000.0
	);
}

double lc_benchmark_start_time( lc_benchmark_t bm )
{
	return bm->start;
}

double lc_benchmark_end_time( lc_benchmark_t bm )
{
	return bm->end;
}

double lc_benchmark_total_time( lc_benchmark_t bm )
{
	return (bm->end - bm->start) * 1000.0;
}
