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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "vector.h"


#if defined(VECTOR_DESTROY_CHECK) || defined(DESTROY_CHECK_ALL)
	#define DESTROY_CHECK( code ) \
		if( p_vector->destroy ) \
		{ \
			code \
		}
#else
	#define DESTROY_CHECK( code ) \
		code 
#endif

/*
 * vector - A growable array of elements.
 */
boolean vector_create( vector_t *p_vector, size_t element_size, size_t size, vector_element_function destroy_callback, alloc_function alloc, free_function free )
{
	assert( p_vector );

	p_vector->element_size = element_size;
	p_vector->array_size   = size;
	p_vector->size         = 0L;
	p_vector->destroy      = destroy_callback;
	p_vector->alloc        = alloc;
	p_vector->free         = free;
	p_vector->array        = p_vector->alloc( vector_element_size(p_vector) * vector_array_size(p_vector) );

	#ifdef _DEBUG_VECTOR
	memset( p_vector->array, 0, vector_element_size(p_vector) * vector_array_size(p_vector) );
	#endif

	assert( p_vector->array );

	return p_vector->array != NULL;
}

void vector_destroy( vector_t *p_vector )
{
	assert( p_vector );

	vector_clear( p_vector );

	p_vector->free( p_vector->array );
	

	#ifdef _DEBUG_VECTOR
	p_vector->element_size = 0L;
	p_vector->array        = NULL;
	p_vector->array_size   = 0L;
	p_vector->size         = 0L;
	p_vector->destroy      = NULL;
	#endif
}

void *vector_pushx( vector_t *p_vector )
{
	void *result;

	assert( p_vector );

	/* grow the array if needed */
	if( vector_size(p_vector) >= vector_array_size(p_vector) )
	{
		size_t new_size      = 2 * p_vector->array_size + 1;
		p_vector->array_size = new_size;
		p_vector->array      = realloc( p_vector->array, p_vector->element_size * vector_array_size(p_vector) );
		assert( p_vector->array );
	}

	#ifdef _DEBUG_VECTOR
	memset( vector_array(p_vector) + vector_size(p_vector) * vector_element_size(p_vector), 0, vector_element_size(p_vector) );
	#endif
	
	result = (void *)( vector_array(p_vector) + vector_size(p_vector) * vector_element_size(p_vector));

	p_vector->size++;
	return result;
}

boolean vector_push( vector_t *p_vector, void *data )
{
	assert( p_vector );

	/* grow the array if needed */
	if( vector_size(p_vector) >= vector_array_size(p_vector) )
	{
		size_t new_size      = 2 * p_vector->array_size + 1;
		p_vector->array_size = new_size;
		p_vector->array      = realloc( p_vector->array, p_vector->element_size * vector_array_size(p_vector) );
		assert( p_vector->array );
	}

	#ifdef _DEBUG_VECTOR
	memset( vector_array(p_vector) + vector_size(p_vector) * vector_element_size(p_vector), 0, vector_element_size(p_vector) );
	#endif

	memcpy( vector_array(p_vector) + (vector_size(p_vector) * vector_element_size(p_vector)), data, vector_element_size(p_vector) );
	p_vector->size++;

	return p_vector->array != NULL;
}

boolean vector_pop( vector_t *p_vector )
{
	void *element;
	boolean result = TRUE;
	assert( p_vector );

	element = vector_array(p_vector) + (vector_size(p_vector) - 1) * vector_element_size(p_vector);

	#ifdef _DEBUG_VECTOR
	memset( vector_array(p_vector) + vector_size(p_vector) * vector_element_size(p_vector), 0, vector_element_size(p_vector) );
	#endif

	DESTROY_CHECK(
		result  = p_vector->destroy( element );
	);

	p_vector->size--;
	return result;
}

#if (!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L)) || defined(_DEBUG_VECTOR)
void* vector_get( vector_t *p_vector, size_t index )
{
	assert( index >= 0 );
	assert( index < vector_size(p_vector) );
	return (void*)(vector_array(p_vector) + vector_element_size(p_vector) * (index));
}

void vector_set( vector_t *p_vector, size_t index, void *data )
{
	assert( p_vector && data );
	assert( index >= 0 );
	assert( index < vector_size(p_vector) );
	memcpy( vector_array(p_vector) + (vector_size(p_vector) * vector_element_size(p_vector)), data, vector_element_size(p_vector) );
}
#endif

boolean vector_resize( vector_t *p_vector, size_t new_size )
{
	boolean result = TRUE;

	if( vector_size(p_vector) > new_size )
	{
		while( vector_size(p_vector) > new_size )
		{
			vector_pop( p_vector );
		}

		p_vector->array_size = new_size;
		p_vector->array      = realloc( p_vector->array, p_vector->element_size * vector_array_size(p_vector) );

		result = p_vector->array != NULL;
	}

	return result;
}

void vector_clear( vector_t *p_vector )
{
	assert( p_vector );

	while( !vector_is_empty(p_vector) )
	{
		vector_pop( p_vector );
	}
}

boolean vector_serialize( vector_t *p_vector, FILE *file, vector_serialize_function func )
{
	if( fwrite( &p_vector->size, sizeof(size_t), 1, file ) == 1 )
	{
		if( !func )
		{
			if( fwrite( p_vector->array, p_vector->element_size, p_vector->size, file ) == p_vector->size )
			{
				return TRUE;	
			}
		}
		else
		{
			/* User passed serialization function */
			return func( p_vector->array );
		}
	}

	return FALSE;
}

boolean vector_unserialize( vector_t *p_vector, FILE *file, vector_unserialize_function func )
{
	size_t new_size = 0;

	if( fread( &new_size, sizeof(size_t), 1, file ) == 1 )
	{
		vector_resize( p_vector, new_size );
	
		if( !func )	
		{
			if( fread( p_vector->array, p_vector->element_size, p_vector->size, file ) == p_vector->size )
			{
				return TRUE;
			}
		}
		else
		{
			/* User passed unserialization function */
			return func( p_vector->array );
		}
	}

	return FALSE;
}


/*
 * pvector - A growable array of pointers.
 */
boolean pvector_create( pvector_t *p_vector, size_t size, pvector_element_function destroy_callback, alloc_function alloc, free_function free )
{
	assert( p_vector );

	p_vector->array_size = size;
	p_vector->size       = 0L;
	p_vector->destroy    = destroy_callback;
	p_vector->alloc        = alloc;
	p_vector->free         = free;
	p_vector->array      = p_vector->alloc( sizeof(void *) * pvector_array_size(p_vector) );

	#ifdef _DEBUG_VECTOR
	memset( p_vector->array, 0, sizeof(void *) * pvector_array_size(p_vector) );
	#endif

	assert( p_vector->array );

	return p_vector->array != NULL;
}

void pvector_destroy( pvector_t *p_vector )
{
	assert( p_vector );

	while( !pvector_is_empty(p_vector) )
	{
		pvector_pop( p_vector );
	}

	p_vector->free( p_vector->array );

	#ifdef _DEBUG_VECTOR
	p_vector->array      = NULL;
	p_vector->array_size = 0L;
	p_vector->size       = 0L;
	p_vector->destroy    = NULL;
	#endif
}

boolean pvector_push( pvector_t *p_vector, void *element )
{
	assert( p_vector );

	/* grow the array if needed */
	if( pvector_size(p_vector) >= pvector_array_size(p_vector) )
	{
		size_t new_size      = 2 * p_vector->array_size + 1;
		p_vector->array_size = new_size;
		p_vector->array      = realloc( p_vector->array, sizeof(void*) * pvector_array_size(p_vector) );
		assert( p_vector->array );
	}

	#ifdef _DEBUG_VECTOR
	memset( pvector_array(p_vector) + pvector_size(p_vector) * sizeof(void *), 0, sizeof(void **) );
	#endif

	p_vector->array[ p_vector->size++ ] = element;

	return p_vector->array != NULL;
}

boolean pvector_pop( pvector_t *p_vector )
{
	void *element;
	boolean result = TRUE;
	assert( p_vector );
	assert( pvector_size(p_vector) > 0 );

	element = p_vector->array[ pvector_size(p_vector) - 1 ];

	DESTROY_CHECK(
		result  = p_vector->destroy( element );
	);

	#ifdef _DEBUG_VECTOR
	memset( pvector_array(p_vector) + pvector_size(p_vector) * sizeof(void *), 0, sizeof(void *) );
	#endif

	p_vector->size--;
	return result;
}

#if (!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L)) || defined(_DEBUG_VECTOR)
void* pvector_get( pvector_t *p_vector, size_t index )
{
	assert( index >= 0 );
	assert( index < pvector_size(p_vector) );
	return p_vector->array[ index ];
}

void pvector_set( pvector_t *p_vector, size_t index, void *data )
{
	assert( p_vector && data );
	assert( index >= 0 );
	assert( index < pvector_size(p_vector) );
	p_vector->array[ index ] = data;
}
#endif

boolean pvector_resize( pvector_t *p_vector, size_t new_size )
{
	boolean result = TRUE;

	if( pvector_size(p_vector) > new_size )
	{
		while( pvector_size(p_vector) > new_size )
		{
			pvector_pop( p_vector );
		}

		p_vector->array_size = new_size;
		p_vector->array      = realloc( p_vector->array, sizeof(void*) * pvector_array_size(p_vector) );
		assert( p_vector->array );

		result = p_vector->array != NULL;
	}

	return result;
}

void pvector_clear( pvector_t *p_vector )
{
	assert( p_vector );

	while( !pvector_is_empty(p_vector) )
	{
		pvector_pop( p_vector );
	}
}

