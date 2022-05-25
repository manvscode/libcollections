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
#ifndef _LC_ARRAY_H_
#define _LC_ARRAY_H_
/**
 * @file array.h
 * @brief A fixed-length array that can be resized but does not grow dynamically.
 *
 * @defgroup lc_array Array
 * @ingroup Collections
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "alloc.h"

/**
 * lc_array_serialize_fxn_t is a function pointer for the array's
 * serialization function.
 */
typedef bool (*lc_array_serialize_fxn_t)( void *p_array );
/**
 * lc_array_unserialize_fxn_t is a function pointer for the array's
 * deserialization function.
 */
typedef bool (*lc_array_unserialize_fxn_t)( void *p_array );

/**
 * lc_array_t is a fixed-length array collection object.
 */
typedef struct lc_array {
	lc_alloc_fxn_t alloc;
	lc_free_fxn_t  free;

	uint8_t*  arr;
	size_t element_size;
	size_t size;
} lc_array_t;


/**
 * Create a fixed-length array collection object.
 *
 * @param p_array An lc_array_t object.
 * @param element_size The size of each element in the array.
 * @param size The length of the array.
 * @param alloc The memory allocation function to use when creating
 *              the array.
 * @param free The memory deallocation function to use when freeing
 *             the array.
 * @return true on success, false on failure.
 */
bool lc_array_create ( lc_array_t* p_array, size_t element_size, size_t size, lc_alloc_fxn_t alloc, lc_free_fxn_t free );

/**
 * Destroy a fixed-length array collection object.
 *
 * @param p_array An lc_array_t object.
 */
void lc_array_destroy ( lc_array_t* p_array );

/**
 * Resize a fixed-length array object. The new size can be
 * larger or smaller than the current size.
 *
 * @param p_array An lc_array_t object.
 * @param new_size The new size of the array.
 * @return true on success, false on failure.
 */
bool lc_array_resize ( lc_array_t* p_array, size_t new_size );
/**
 * Serialize a fixed-length array object to a file stream.
 *
 * @param p_array An lc_array_t object.
 * @param file The file stream to write to.
 * @param func The serialization callback.
 * @return true on success, false on failure.
 */
bool lc_array_serialize ( lc_array_t* p_array, FILE* file, lc_array_serialize_fxn_t func );
/**
 * Unserialize a fixed-length array object from a file stream.
 *
 * @param p_array An lc_array_t object.
 * @param file The file stream to read from.
 * @param func The unserialization callback.
 * @return true on success, false on failure.
 */
bool lc_array_unserialize ( lc_array_t* p_array, FILE* file, lc_array_unserialize_fxn_t func );
/**
 * Get the array's base pointer.
 *
 * @param p_array An lc_array_t object.
 * @return Pointer to array's base elment (i.e address of 0th element).
 */
#define lc_array_base( p_array ) ((p_array)->arr)
/**
 * Get the array's element size.
 *
 * @param p_array An lc_array_t object.
 * @return Size of each element.
 */
#define lc_array_element_size( p_array )  ((p_array)->element_size)
/**
 * Get the array's length
 *
 * @param p_array An lc_array_t object.
 * @return Return the length of the array.
 */
#define lc_array_size( p_array ) ((p_array)->size)

#if !defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L) /* Not C99 */
void* lc_array_element ( lc_array_t* p_array, size_t index );
#else
#include <assert.h>
static inline void* lc_array_element( lc_array_t* p_array, size_t index )
{
	assert( index >= 0 );
	assert( index < lc_array_size(p_array) );
	return (void*)(lc_array_base(p_array) + lc_array_element_size(p_array) * (index));
}
#endif

/**
 * Get a pointer to an element.
 *
 * @param p_array An lc_array_t object.
 * @param index The element's index.
 * @return Returns a pointer to a element at a certain index.
 */
#define lc_array_elem( p_array, index, type ) ((type *) lc_array_element( p_array, index ))


#ifdef __cplusplus
}
#endif
#endif /* _LC_ARRAY_H_ */
