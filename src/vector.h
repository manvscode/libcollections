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
#define VECTOR_GROW_AMOUNT(array)      (0.5f * vector_size(array) + 1)
#endif
#endif

#define vector_create(array, size)          ((array) = NULL, vector_resize(array, size))
#define vector_destroy(array)               ((array) ? free(vector_raw(array)), 0 : 0)
#define vector_raw(array)                   (((size_t*)(array)) - 2)
#define vector_length(array)                ((array) ? vector_raw(array)[ 0 ] : 0)
#define vector_size(array)                  ((array) ? vector_raw(array)[ 1 ] : 0)
#define vector_first(array)                 ((array)[ 0 ])
#define vector_last(array)                  ((array)[ vector_length(array) - 1 ])
#define vector_need_grow(array)             (!array || vector_length(array) >= vector_size(array))
#define vector_grow(array, size)            ((vector_need_grow(array)) ? vector_resize(array, (size)) : 0)
#define vector_resize(array, size)          ((array) = __vector_resize(array, sizeof(*array), (size)))
#define vector_clear(array)                 (vector_l(array) = 0)
#define vector_push(array, item)            (vector_grow(array, vector_size(array) + VECTOR_GROW_AMOUNT(array)), array[ vector_l(array)++ ] = item)
#define vector_pop(array)                   (vector_l(array)--)
#define vector_pushn(array, item, amount)   (vector_grow(array, vector_size(array) + amount), array[ vector_l(array)++ ] = item)
#define vector_popn(array, item, amount)    (vector_grow(array, vector_size(array) + amount), vector_l(array)--)
#define vector_l(array)                     (vector_raw(array)[ 0 ])
#define vector_s(array)                     (vector_raw(array)[ 1 ])

void* __vector_resize( void* array, size_t element_size, size_t new_size );

#ifdef __cplusplus
}
#endif
#endif /* _VECTOR_H_ */
