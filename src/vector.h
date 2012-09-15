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
#ifndef _VECTOR_H_
#define _VECTOR_H_
#ifdef __cplusplus
extern "C" {
#endif 

#include <stddef.h>
#include <string.h>
#include "types.h"

#ifndef VECTOR_INITIAL_ARRAY_SIZE
#define VECTOR_INITIAL_ARRAY_SIZE    250
#endif
typedef boolean (*vector_serialize_function)   ( void *p_array );
typedef boolean (*vector_unserialize_function) ( void *p_array );
typedef boolean (*vector_element_function)     ( void *data );

typedef struct vector {
	size_t element_size;
	byte*  array;
	size_t array_size;
	size_t size;
	vector_element_function destroy;
} vector_t;

/*
 * vector - A growable array of elements.
 */
#define vector_simple_create( p_vector, element_size, VECTOR_INITIAL_ARRAY_SIZE, destroy_callback )\
	vector_create( p_vector, element_size, size, destroy_callback )

boolean      vector_create      ( vector_t *p_vector, size_t element_size, size_t size, vector_element_function destroy_callback );
void         vector_destroy     ( vector_t *p_vector );
void*        vector_pushx       ( vector_t *p_vector );
boolean      vector_push        ( vector_t *p_vector, void *data );
boolean      vector_pop         ( vector_t *p_vector );
boolean      vector_resize      ( vector_t *p_vector, size_t new_size );
void         vector_clear       ( vector_t *p_vector );
boolean      vector_serialize   ( vector_t *p_vector, FILE *file, vector_serialize_function func );
boolean      vector_unserialize ( vector_t *p_vector, FILE *file, vector_unserialize_function func );

#define vector_element_size( p_vector )  ((p_vector)->element_size)
#define vector_array( p_vector )         ((p_vector)->array)
#define vector_array_size( p_vector )    ((p_vector)->array_size)
#define vector_size( p_vector )          ((p_vector)->size)
#define vector_is_empty( p_vector )      ((p_vector)->size <= 0)
#define vector_peek( p_vector )          (vector_get(p_vector, vector_size(p_vector) - 1))

#if (!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L)) || defined(_DEBUG_VECTOR)
void* vector_get         ( vector_t *p_vector, size_t index );
void  vector_set         ( vector_t *p_vector, size_t index, void *data );
#else
#include <assert.h>
inline void* vector_get( vector_t *p_vector, size_t index )
{
	assert( index >= 0 );
	assert( index < vector_size(p_vector) );
	return (void*)(vector_array(p_vector) + vector_element_size(p_vector) * (index));
}

inline void vector_set( vector_t *p_vector, size_t index, void *data )
{
	assert( data != NULL );
	assert( index >= 0 );
	assert( index < vector_size(p_vector) );
	memcpy( vector_array(p_vector) + (vector_size(p_vector) * vector_element_size(p_vector)), data, vector_element_size(p_vector) );
}
#endif

#if 0
#define vector_get( p_vector, index ) ((void*)(vector_array(p_vector) + vector_element_size(p_vector) * (index)))
#endif

/*
 * pvector - A growable array of pointers.
 */
typedef boolean (*pvector_element_function)( void *data );

typedef struct pvector {
	void** array;
	size_t array_size;
	size_t size;
	vector_element_function destroy;
} pvector_t;

boolean      pvector_create     ( pvector_t *p_vector, size_t size, pvector_element_function destroy_callback );
void         pvector_destroy    ( pvector_t *p_vector );
boolean      pvector_push       ( pvector_t *p_vector, void *data );
boolean      pvector_pop        ( pvector_t *p_vector );
boolean      pvector_resize     ( pvector_t *p_vector, size_t new_size );
void         pvector_clear      ( pvector_t *p_vector );

#define pvector_array( p_vector )       ((p_vector)->array)
#define pvector_array_size( p_vector )  ((p_vector)->array_size)
#define pvector_size( p_vector )        ((p_vector)->size)
#define pvector_is_empty( p_vector )    ((p_vector)->size <= 0)
#define pvector_peek( p_vector )        (pvector_get(p_vector, vector_size(p_vector) - 1))

#if (!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L)) || defined(_DEBUG_VECTOR)
void* pvector_get        ( pvector_t *p_vector, size_t index );
void  pvector_set        ( pvector_t *p_vector, size_t index, void *data );
#else
#include <assert.h>
inline void* pvector_get( pvector_t *p_vector, size_t index )
{
	assert( index >= 0 );
	assert( index < pvector_size(p_vector) );
	return p_vector->array[ index ];
}

inline void pvector_set( pvector_t *p_vector, size_t index, void *data )
{
	assert( p_vector && data );
	assert( index >= 0 );
	assert( index < pvector_size(p_vector) );
	p_vector->array[ index ] = data;
}
#endif

/*
 * stack - A first in, last out data structure.
 */
typedef vector_t stack;

#define stack_create         vector_create   
#define stack_destroy        vector_destroy 
#define stack_push           vector_push    
#define stack_pop            vector_pop     
#define stack_element_size   vector_element_size 
#define stack_array          vector_array
#define stack_array_size     vector_array_size
#define stack_size           vector_size
#define stack_is_empty       vector_is_empty

#ifdef __cplusplus
}
#endif 
#endif /* _VECTOR_H_ */
