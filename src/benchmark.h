/*
 * Copyright (C) 2010 by Joseph A. Marrero.  http://www.manvscode.com/
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
#ifndef _LC_BENCHMARK_H_
#define _LC_BENCHMARK_H_


/**
 * @file bench-mark.h
 * @brief Bench marking routines for timing code and
 * measuring performance.
 *
 * @defgroup lc_benchmark Benchmarking
 * @ingroup Other
 * @{
 */


#ifdef __cplusplus
extern "C" {
#endif

/**
 * lc_benchmark_t is an opaque handle to a bench marking
 * object.
 */
struct lc_benchmark;
typedef struct lc_benchmark* lc_benchmark_t;

/**
 * Create a bench marking object.
 *
 * @param description A description used for writing trace
 *                    messages to stdout.
 * @return Returns NULL on failure.
 */
lc_benchmark_t lc_benchmark_create ( const char *description );

/**
 * Destroy a bench marking object.
 *
 * @param bm A handle to a bench marking object.
 */
void lc_benchmark_destroy ( lc_benchmark_t bm );
/**
 * Start timing.
 *
 * @param bm A handle to a bench marking object.
 */
void lc_benchmark_start ( lc_benchmark_t bm );
/**
 * End timing.
 *
 * @param bm A handle to a bench marking object.
 */
void lc_benchmark_end ( lc_benchmark_t bm );
/**
 * Report timing measurements to standard output.
 *
 * @param bm A handle to a bench marking object.
 */
void lc_benchmark_report ( lc_benchmark_t bm );
/**
 * Get the start time.
 *
 * @param bm A handle to a bench marking object.
 * @return The start time.
 */
double lc_benchmark_start_time ( lc_benchmark_t bm );
/**
 * Get the end time.
 *
 * @param bm A handle to a bench marking object.
 * @return The end time.
 */
double lc_benchmark_end_time ( lc_benchmark_t bm );
/**
 * Get the total time.  This is the differance of the
 * end time and stat time.
 *
 * @param bm A handle to a bench marking object.
 * @return The end time.
 */
double lc_benchmark_total_time ( lc_benchmark_t bm );


#ifdef __cplusplus
}
#endif
#endif /* _LC_BENCHMARK_H_ */
