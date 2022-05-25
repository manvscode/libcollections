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
#ifndef _LC_VECTOR_H_
#define _LC_VECTOR_H_
/**
 * @file vector.h
 * @brief A groweable vector collection.
 *
 * A vector is similar to a classic array but with the benefit of growing in size
 * when needed.
 *
 * @defgroup lc_vector Vector
 * @ingroup Collections
 *
 * @warning The underlying memory used to store elements is relocatable. If pointers
 * are used to refer to items in the vector, then the pointers may become invalid if
 * the vector is grown and moved to another memory location (see realloc() for more
 * information).
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#ifndef LC_VECTOR_GROW_AMOUNT
# if 0
# define LC_VECTOR_GROW_AMOUNT(array)      (13)
# else
# define LC_VECTOR_GROW_AMOUNT(array)      (0.5f * lc_vector_capacity(array) + 1)
#endif
#endif

/**
 *  Create a vector.
 */
#define lc_vector_create(array, capacity)       ((array) = NULL, lc_vector_reserve(array, capacity))
/**
 *  Destroy a vector.
 */
#define lc_vector_destroy(array)                (free(lc_vector_raw(array)))

/**
 *  Get a vector's raw base pointer.  This is the address of where the size
 *  and capacity are stored.
 */
#define lc_vector_raw(array)                    (((size_t*)(array)) - 2)
/**
 *  Get the size of a vector.
 */
#define lc_vector_size(array)                   ((array) ? lc_vector_s(array) : 0)
/**
 *  Get the capacity of a vector.
 */
#define lc_vector_capacity(array)               ((array) ? lc_vector_c(array) : 0)
/**
 *  Get the first element in a vector.
 */
#define lc_vector_first(array)                  ((array)[ 0 ])
/**
 *  Get the last element in a vector.
 */
#define lc_vector_last(array)                   ((array)[ lc_vector_size(array) - 1 ])
/**
 *  Tests whether a vector needs to grow.
 */
#define lc_vector_need_grow(array)              (!array || lc_vector_size(array) >= lc_vector_capacity(array))
/**
 *  Grows a vector to a new capacity if needed.
 */
#define lc_vector_grow(array, capacity)         ((lc_vector_need_grow(array)) ? lc_vector_reserve(array, capacity) : true)
/**
 *  Reserves enough memory to store a certain amount of elements
 *  in the vector.
 */
#define lc_vector_reserve(array, capacity)      lc_vector_reserve_capacity((void**)&array, sizeof(*array), capacity)
/**
 *  Empties the vector. No memory is freed by this operation.
 */
#define lc_vector_clear(array)                  (lc_vector_s(array) = 0)
/**
 *  Appends a new element to a vector.  If the vector is not large enough to store
 *  the element then the vector will be grown by LC_VECTOR_GROW_AMOUNT.
 */
#define lc_vector_push(array, item)             (lc_vector_grow(array, lc_vector_capacity(array) + LC_VECTOR_GROW_AMOUNT(array)) ? ((array)[ lc_vector_s(array)++ ] = (item), true) : false)
#define lc_vector_push_emplace(array)           (lc_vector_grow(array, lc_vector_capacity(array) + LC_VECTOR_GROW_AMOUNT(array)) ? ((lc_vector_s(array)++), true) : false)
#define lc_vector_pushn(array, item, amount)    (lc_vector_grow(array, lc_vector_capacity(array) + amount) ? ((array)[ lc_vector_s(array)++ ] = (item), true) : false)
/**
 *  Removes an element from the end of a vector.
 */
#define lc_vector_pop(array)                    (lc_vector_s(array)--)
/**
 *  Get the size of a vector. This does not do any checks.
 */
#define lc_vector_s(array)                      (lc_vector_raw(array)[ 0 ])
/**
 *  Get the capacity of a vector. This does not do any checks.
 */
#define lc_vector_c(array)                      (lc_vector_raw(array)[ 1 ])

/**
 *  Reserves enough memory to store a certain amount of elements
 *  in the vector. When a vector is resized, the memory may be
 *  relocated if it cannot be resized in place.  If the vector is
 *  storing pointers then the vector should store double-pointers.
 *
 *  @param array Double pointer to a vector.
 *  @param element_size The element size of the vector.
 *  @param capacity The capacity that the vector should have.
 *
 *  @return true on success, false on failure.
 */
bool lc_vector_reserve_capacity( void** array, size_t element_size, size_t capacity );

#ifdef __cplusplus
}
#endif
#endif /* _LC_VECTOR_H_ */
