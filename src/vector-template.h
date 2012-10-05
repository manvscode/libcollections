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
#ifndef _VECTOR_TEMPLATE_H_
#define _VECTOR_TEMPLATE_H_
#ifdef __cplusplus
extern "C" {
#endif 

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include "libcollections-config.h"
#include "types.h"


/*
 * vector - A growable array of type.
 */

#define define_vector_type( name, type ) \
	typedef boolean (*vector_##name##_element_function)( type* data );\
	typedef boolean (*vector_##name##_serialize_function)   ( const type *p_array ); \
	typedef boolean (*vector_##name##_unserialize_function) ( const type *p_array ); \
	\
	typedef struct vector_##name { \
		size_t array_size; \
		size_t size; \
		type*  array; \
	} vector_##name##_t; \
	\
	#define vector_##name##_array_size( p_vector )    ((p_vector)->array_size) \
	#define vector_##name##_size( p_vector )          ((p_vector)->size) \
	#define vector_##name##_is_empty( p_vector )      ((p_vector)->size <= 0) \
	#define vector_##name##_array( p_vector )         ((p_vector)->array) \
	#define vector_##name##_peek( p_vector )          (vector_##name##_get(p_vector, vector_##name##_size(p_vector) - 1)) \
	\
	static inline boolean vector_##name##_create( vector_##name##_t *p_vector, size_t size ) \
	{ \
		assert( p_vector ); \
		p_vector->array_size   = size; \
		p_vector->size         = 0L; \
		p_vector->array        = malloc( sizeof(type) * (p_vector->array_size) ); \
		assert( p_vector->array ); \
		return p_vector->array != NULL; \
	} \
	\
	static inline void vector_##name##_destroy( vector_##name##_t *p_vector ) \
	{ \
		assert( p_vector ); \
		vector_##name##_clear( p_vector ); \
		free( p_vector->array ); \
		#ifdef _DEBUG_VECTOR \
		p_vector->array        = NULL; \
		p_vector->array_size   = 0L; \
		p_vector->size         = 0L; \
		#endif \
	} \
	\
	static inline type* vector_##name##_pushx( vector_##name##_t *p_vector ) \
	{ \
		type *result; \
		assert( p_vector ); \
		/* grow the array if needed */ \
		if( vector_##name##_size(p_vector) >= vector_##name##_array_size(p_vector) ) \
		{ \
			size_t new_size      = 2 * p_vector->array_size + 1; \
			p_vector->array_size = new_size; \
			p_vector->array      = realloc( p_vector->array, sizeof(type) * vector_##name##_array_size(p_vector) ); \
			assert( p_vector->array ); \
		} \
		#ifdef _DEBUG_VECTOR \
		memset( vector_array(p_vector) + vector_##name##_size(p_vector), 0, vector_##name##_element_size(p_vector) ); \
		#endif \
		result = vector_##name##_array(p_vector) + vector_##name##_size(p_vector);  \
		p_vector->size++; \
		return result; \
	} \
	\
	static inline boolean vector_##name##_push( vector_##name##_t *p_vector, const type *data ) \
	{ \
		assert( p_vector ); \
		/* grow the array if needed */ \
		if( vector_##name##_size(p_vector) >= vector_##name##_array_size(p_vector) ) \
		{ \
			size_t new_size      = 2 * p_vector->array_size + 1; \
			p_vector->array_size = new_size; \
			p_vector->array      = realloc( p_vector->array, sizeof(type) * vector_##name##_array_size(p_vector) ); \
			assert( p_vector->array ); \
		} \
		#ifdef _DEBUG_VECTOR \
		memset( vector_##name##_array(p_vector) + vector_##name##_size(p_vector), 0, sizeof(type) ); \
		#endif \
		type* dst = vector_##name##_array(p_vector) + vector_##name##_size(p_vector); \
		*dst = *data; /*memcpy( dst, data, sizeof(type) );*/ \
		p_vector->size++; \
		return p_vector->array != NULL; \
	} \
	\
	static inline boolean vector_##name##_pop( vector_##name##_t *p_vector ) \
	{ \
		boolean result = FALSE; \
		assert( p_vector ); \
		if( vector_##name##_size(p_vector) > 0 ) \
		{ \
			type *element = vector_##name##_array(p_vector) + vector_##name##_size(p_vector) - 1); \
			#ifdef _DEBUG_VECTOR \
			memset( vector_##name##_array(p_vector) + (vector_##name##_size(p_vector) - 1), 0, sizeof(type) ); \
			#endif \
			p_vector->size--; \
			result = TRUE; \
		} \
		return result; \
	} \
	\
	static inline type* vector_##name##_get( vector_##name##_t *p_vector, size_t index ) \
	{ \
		assert( index >= 0 ); \
		assert( index < vector_##name##_size(p_vector) ); \
		return vector_##name##_array(p_vector) + index; \
	} \
 	\
	static inline void vector_##name##_set( vector_##name##_t *p_vector, size_t index, const type* data ) \
	{ \
		assert( data != NULL ); \
		assert( index >= 0 ); \
		assert( index < vector_##name##_size(p_vector) ); \
		type* dst = vector_##name##_array(p_vector) + vector_##name##_size(p_vector); \
		*dst = *data; \
		/*memcpy( vector_##name##_array(p_vector) + vector_##name##_size(p_vector), data, sizeof(type) );*/ \
	} \
	\
	boolean vector_##name##_resize( vector_##name##_t *p_vector, size_t new_size ) \
	{ \
		boolean result = TRUE; \
		if( vector_##name##_size(p_vector) > new_size ) \
		{ \
			while( vector_##name##_size(p_vector) > new_size ) \
			{ \
				vector_##name##_pop( p_vector ); \
			} \
			p_vector->array_size = new_size; \
			p_vector->array      = realloc( p_vector->array, sizeof(type) * vector_##name##_array_size(p_vector) ); \
			result               = p_vector->array != NULL; \
		} \
		return result; \
	} \
	\
	void vector_##name##_clear( vector_##name##_t *p_vector ) \
	{ \
		assert( p_vector ); \
		while( !vector_##name##_is_empty(p_vector) ) \
		{ \
			vector_##name##_pop( p_vector ); \
		} \
	} \
	\
	boolean vector_##name##_serialize( vector_##name##_t *p_vector, FILE *file, vector_##name##_serialize_function func ) \
	{ \
		boolean result = FALSE; \
		if( fwrite( &p_vector->size, sizeof(size_t), 1, file ) == 1 ) \
		{ \
			if( !func ) \
			{ \
				if( fwrite( p_vector->array, sizeof(type), p_vector->size, file ) == p_vector->size ) \
				{ \
					result =  TRUE;	 \
				} \
			} \
			else \
			{ \
				/* User passed serialization function */ \
				result =  func( p_vector->array ); \
			} \
		} \
		return result; \
	} \
 	\
	boolean vector_##name##_unserialize( vector_##name##_t *p_vector, FILE *file, vector_##name##_unserialize_function func ) \
	{ \
		boolean result = FALSE; \
		size_t new_size = 0; \
		if( fread( &new_size, sizeof(size_t), 1, file ) == 1 ) \
		{ \
			vector_resize( p_vector, new_size ); \
		 	\
			if( !func )	 \
			{ \
				if( fread( p_vector->array, sizeof(type), p_vector->size, file ) == p_vector->size ) \
				{ \
					result = TRUE; \
				} \
			} \
			else \
			{ \
				/* User passed unserialization function */ \
				result = func( p_vector->array ); \
			} \
		} \
		return result; \
	}

#ifdef __cplusplus
}
#endif 
#endif /* _VECTOR_TEMPLATE_H_ */
