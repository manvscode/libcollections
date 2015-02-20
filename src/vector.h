/*
 * Copyright (C) 2010-2014 by Joseph A. Marrero.  http://www.manvscode.com/
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
#include <assert.h>
#ifndef VECTOR_GROW_AMOUNT
#if 0
#define VECTOR_GROW_AMOUNT(array)      (13)
#else
#define VECTOR_GROW_AMOUNT(array)      (0.5f * vector_capacity(array) + 1)
#endif
#endif

#define vector_create(array, capacity)       ((array) = NULL, vector_reserve(array, capacity))
#define vector_destroy(array)                ((array) ? free(vector_raw(array)), 0 : 0)
#define vector_raw(array)                    (((size_t*)(array)) - 2)
#define vector_size(array)                   ((array) ? vector_s(array) : 0)
#define vector_capacity(array)               ((array) ? vector_c(array) : 0)
#define vector_first(array)                  ((array)[ 0 ])
#define vector_last(array)                   ((array)[ vector_size(array) - 1 ])
#define vector_need_grow(array)              (!array || vector_size(array) >= vector_capacity(array))
#define vector_grow(array, capacity)         ((vector_need_grow(array)) ? vector_reserve(array, (capacity)) : 0)
#define vector_reserve(array, capacity)      ((array) = __vector_set_capacity(array, sizeof(*array), (capacity)))
#define vector_clear(array)                  (vector_s(array) = 0)
#define vector_push(array, item)             (vector_grow(array, vector_capacity(array) + VECTOR_GROW_AMOUNT(array)), array[ vector_s(array)++ ] = item)
#define vector_push_emplace(array)           (vector_grow(array, vector_capacity(array) + VECTOR_GROW_AMOUNT(array)), vector_s(array)++)
#define vector_pop(array)                    (vector_s(array)--)
#define vector_pushn(array, item, amount)    (vector_grow(array, vector_capacity(array) + amount), array[ vector_s(array)++ ] = item)
#define vector_popn(array, item, amount)     (vector_grow(array, vector_capacity(array) + amount), vector_s(array)--)
#define vector_s(array)                      (vector_raw(array)[ 0 ])
#define vector_c(array)                      (vector_raw(array)[ 1 ])

void* __vector_set_capacity( void* array, size_t element_size, size_t capacity );

#ifdef __cplusplus
}
#endif
#endif /* _VECTOR_H_ */
