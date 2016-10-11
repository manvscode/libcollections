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
#include <stdlib.h>
#include <string.h>
#include "vector.h"

/*
 * Pool should grow linearly.
 */
#ifndef POOL_GROWTH_LINEAR
# define POOL_GROWTH_LINEAR(amount)     (amount)
#endif
/*
 * Pool should not grow.
 */
#ifndef POOL_GROWTH_NONE
# define POOL_GROWTH_NONE               (0)
#endif
/*
 * Pool should grow expontially.
 */
#ifndef POOL_GROWTH_EXPONENTIAL
# define POOL_GROWTH_EXPONENTIAL        (-1)
#endif
/*
 * Initialize memory to a certain value.
 */
#ifdef POOL_INITIALIZE_MEMORY
# ifndef POOL_UNUSED_MEMORY_VALUE
# define POOL_UNUSED_MEMORY_VALUE       (-1) /* Initialize memory to 0xFF */
# endif
#endif

/*
 * Declare an object pool for a specific data type.
 */
#define POOL_DECL( prefix, data_type ) \
	struct prefix##_pool; \
	typedef struct prefix##_pool prefix##_pool_t; \
	\
	prefix##_pool_t* prefix##_pool_create  ( size_t count, int growth ); \
	void             prefix##_pool_destroy ( prefix##_pool_t** pool ); \
	data_type**      prefix##_pool_obtain  ( prefix##_pool_t* pool ); \
	void             prefix##_pool_release ( prefix##_pool_t* pool, data_type** obj ); \
	void             prefix##_pool_shrink( prefix##_pool_t* pool );


/*
 * Implement an object pool for a specific data type.
 */
#define POOL_IMPL( prefix, data_type ) \
	struct prefix##_pool { \
		int growth; \
		data_type*  items;      /* data_type  items[ 10 ]; */ \
		data_type** available;  /* data_type* available[ 10 ]; */ \
	}; \
	\
	static inline void prefix##_pool_grow( prefix##_pool_t* pool ); \
	\
	prefix##_pool_t* prefix##_pool_create( size_t count, int growth ) \
	{ \
		prefix##_pool_t* pool = (prefix##_pool_t*) malloc( sizeof(prefix##_pool_t) ); \
		\
		if( count == 0 ) \
		{ \
			/* 1 is the minimum */ \
			count = 1; \
		} \
		\
		if( pool ) \
		{ \
			pool->growth    = growth; \
			lc_vector_create( pool->items, count ); \
			lc_vector_create( pool->available, count ); \
			\
			for( size_t i = 0; i < count; i++ ) \
			{ \
				lc_vector_push( pool->available, &pool->items[ i ] ); \
			} \
		} \
		\
		return pool; \
	} \
	\
	void prefix##_pool_destroy( prefix##_pool_t** pool ) \
	{ \
		if( pool && *pool ) \
		{ \
			lc_vector_destroy( (*pool)->items	); \
			lc_vector_destroy( (*pool)->available ); \
			free( *pool ); \
			*pool = NULL; \
		} \
	} \
	\
	data_type** prefix##_pool_obtain( prefix##_pool_t* pool ) \
	{ \
		data_type** result = NULL; \
		\
		if( pool ) \
		{ \
			retry_obtain: \
			{ \
				const size_t available_count = lc_vector_size(pool->available); \
				\
				if( available_count > 0 ) \
				{ \
					data_type** obj = &lc_vector_last( pool->available ); \
					lc_vector_pop( pool->available ); \
					result = obj; \
				} \
				else if( pool->growth ) \
				{ \
					prefix##_pool_grow( pool ); \
					goto retry_obtain; \
				} \
			} \
		} \
		\
		return result; \
	} \
	\
	void prefix##_pool_release( prefix##_pool_t* pool, data_type** obj ) \
	{ \
		if( pool && obj && *obj ) \
		{ \
			lc_vector_push( pool->available, (data_type*) *obj ); \
			*obj = NULL; \
		} \
	} \
	\
	void prefix##_pool_grow( prefix##_pool_t* pool ) \
	{ \
		/* We ran out of items so we need to grow the */ \
		/* pool.                                      */ \
		size_t old_capacity = lc_vector_capacity( pool->items ); \
		size_t new_capacity = pool->growth > 0 ? (old_capacity + (size_t)pool->growth) : (1.5f * old_capacity + 1); \
		\
		data_type* old_base = pool->items; \
		\
		pool->items     = lc_vector_reserve( pool->items, new_capacity ); \
		pool->available = lc_vector_reserve( pool->available, new_capacity ); \
		\
		if( old_base != pool->items ) \
		{ \
			/* Pool was relocated somewhere else in memory. */ \
			/* We need to update the previous pointers.     */ \
			for( size_t i = 0; i < old_capacity; i++ ) \
			{ \
				pool->available[ i ] = &pool->items[ i ]; \
			} \
		} \
		\
		for( size_t i = old_capacity; i < new_capacity; i++ ) \
		{ \
			lc_vector_push( pool->available, &pool->items[ i ] ); \
		} \
	}

