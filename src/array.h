/*
 * Copyright (C) 2010 by Joseph A. Marrero and Shrewd LLC. http://www.manvscode.com/
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
#ifndef _ARRAY_H_
#define _ARRAY_H_
#ifdef __cplusplus
extern "C" {
#endif 

#include <stddef.h>
#include "types.h"
#include "config.h"

typedef boolean (*array_serialize_function)( void *p_array );
typedef boolean (*array_unserialize_function)( void *p_array );

typedef struct array {
	byte*  arr;
	size_t element_size;
	size_t size;
} array_t;

boolean   array_create      ( array_t *p_array, size_t element_size, size_t size );
void      array_destroy     ( array_t *p_array );
boolean   array_resize      ( array_t *p_array, size_t new_size );
boolean   array_serialize   ( array_t *p_array, FILE *file, array_serialize_function func );
boolean   array_unserialize ( array_t *p_array, FILE *file, array_unserialize_function func );
#define   array_base( p_array )          ((p_array)->arr)
#define   array_element_size( p_array )  ((p_array)->element_size)
#define   array_size( p_array )          ((p_array)->size)

#if !defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L)
void*     array_element     ( array_t *p_array, size_t index );
#else
#include <assert.h>
static inline void* array_element( array_t *p_array, size_t index )
{
	assert( index >= 0 );
	assert( index < array_size(p_array) );
	return (void*)(array_base(p_array) + array_element_size(p_array) * (index));
}
#endif

#define array_elem( p_array, index, type ) ((type *) array_element( p_array, index ))


#ifdef __cplusplus
}
#endif 
#endif /* _ARRAY_H_ */
