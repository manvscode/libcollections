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
#ifndef _DYNARRAY_H_
#define _DYNARRAY_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>

#ifndef DYNARRAY_GROW_AMOUNT
#if 0
#define DYNARRAY_GROW_AMOUNT(array)      (13)
#else
#define DYNARRAY_GROW_AMOUNT(array)      (0.5f * lc_dynarray_size(array) + 1)
#endif
#endif

#define lc_dynarray_destroy(array)               ((array) ? free(lc_dynarray_raw(array)), 0 : 0)
#define lc_dynarray_raw(array)                   (((size_t*)(array)) - 2)
#define lc_dynarray_length(array)                ((array) ? lc_dynarray_raw(array)[ 0 ] : 0)
#define lc_dynarray_size(array)                  ((array) ? lc_dynarray_raw(array)[ 1 ] : 0)
#define lc_dynarray_first(array)                 ((array)[ 0 ])
#define lc_dynarray_last(array)                  ((array)[ lc_dynarray_length(array) - 1 ])
#define lc_dynarray_need_grow(array)             (!array || lc_dynarray_length(array) >= lc_dynarray_size(array))
#define lc_dynarray_grow(array, size)            ((lc_dynarray_need_grow(array)) ? lc_dynarray_resize(array, (size)) : 0)
#define lc_dynarray_resize(array, size)          (array = __lc_dynarray_resize(array, sizeof(*array), (size)))
#define lc_dynarray_push(array, item)            (lc_dynarray_grow(array, lc_dynarray_size(array) + DYNARRAY_GROW_AMOUNT(array)), array[ lc_dynarray_l(array)++ ] = item)
#define lc_dynarray_pop(array)                   (lc_dynarray_l(array)--)
#define lc_dynarray_pushn(array, item, amount)   (lc_dynarray_grow(array, lc_dynarray_size(array) + amount), array[ lc_dynarray_l(array)++ ] = item)
#define lc_dynarray_popn(array, item, amount)    (lc_dynarray_grow(array, lc_dynarray_size(array) + amount), lc_dynarray_l(array)--)
#define lc_dynarray_l(array)                     (lc_dynarray_raw(array)[ 0 ])
#define lc_dynarray_s(array)                     (lc_dynarray_raw(array)[ 1 ])

void* __lc_dynarray_resize( void* array, size_t element_size, size_t new_size );

#ifdef __cplusplus
}
#endif
#endif /* _DYNARRAY_H_ */
