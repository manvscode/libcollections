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
#ifndef _VECTOR_H_
#define _VECTOR_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#ifndef VECTOR_GROW_AMOUNT
#if 0
#define VECTOR_GROW_AMOUNT(array)      (13)
#else
#define VECTOR_GROW_AMOUNT(array)      (0.5f * lc_vector_capacity(array) + 1)
#endif
#endif

/**
 *  Creates a vector.
 */
#define lc_vector_create(array, capacity)       ((array) = NULL, lc_vector_reserve(array, capacity))
/**
 *  Destroys a vector.
 */
#define lc_vector_destroy(array)                (free(lc_vector_raw(array)))
#define lc_vector_raw(array)                    (((size_t*)(array)) - 2)
#define lc_vector_size(array)                   ((array) ? lc_vector_s(array) : 0)
#define lc_vector_capacity(array)               ((array) ? lc_vector_c(array) : 0)
#define lc_vector_first(array)                  ((array)[ 0 ])
#define lc_vector_last(array)                   ((array)[ lc_vector_size(array) - 1 ])
#define lc_vector_need_grow(array)              (!array || lc_vector_size(array) >= lc_vector_capacity(array))
#define lc_vector_grow(array, capacity)         ((lc_vector_need_grow(array)) ? lc_vector_reserve(array, capacity) : true)
#define lc_vector_reserve(array, capacity)      lc_vector_reserve_capacity((void**)&array, sizeof(*array), capacity)
#define lc_vector_clear(array)                  (lc_vector_s(array) = 0)
#define lc_vector_push(array, item)             (lc_vector_grow(array, lc_vector_capacity(array) + VECTOR_GROW_AMOUNT(array)) ? ((array)[ lc_vector_s(array)++ ] = (item), true) : false)
#define lc_vector_push_emplace(array)           (lc_vector_grow(array, lc_vector_capacity(array) + VECTOR_GROW_AMOUNT(array)) ? ((lc_vector_s(array)++), true) : false)
#define lc_vector_pushn(array, item, amount)    (lc_vector_grow(array, lc_vector_capacity(array) + amount) ? ((array)[ lc_vector_s(array)++ ] = (item), true) : false)
#define lc_vector_pop(array)                    (lc_vector_s(array)--)
#define lc_vector_s(array)                      (lc_vector_raw(array)[ 0 ])
#define lc_vector_c(array)                      (lc_vector_raw(array)[ 1 ])

bool lc_vector_reserve_capacity( void** array, size_t element_size, size_t capacity );

#ifdef __cplusplus
}
#endif
#endif /* _VECTOR_H_ */
