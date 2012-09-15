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
#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_
#ifdef __cplusplus
extern "C" {
#endif 

#include <stddef.h>
#include "types.h"
#ifdef USE_ALLOCATORS
#include "alloc.h"
#endif

/* Each size is a prime number */
#ifndef HASH_MAP_SIZE_SMALL 
#define HASH_MAP_SIZE_SMALL         (37)
#endif
#ifndef HASH_MAP_SIZE_MEDIUM 
#define HASH_MAP_SIZE_MEDIUM        (1031)
#endif
#ifndef HASH_MAP_SIZE_LARGE
#define HASH_MAP_SIZE_LARGE         (5153)
#endif
#ifndef HASH_MAP_SIZE_EXTRA_LARGE
#define HASH_MAP_SIZE_EXTRA_LARGE   (7919)
#endif

#ifndef HASH_MAP_DEFAULT_LOAD_FACTOR
#define HASH_MAP_DEFAULT_LOAD_FACTOR     (0.7)
#endif

#ifndef HASH_MAP_THRESHOLD
#define HASH_MAP_THRESHOLD               (0.7)
#endif



typedef size_t  (*hash_map_hash_function)    ( const void *key );
typedef boolean (*hash_map_element_function) ( void *key, void *value );
typedef int     (*hash_map_compare_function) ( const void *left, const void *right );

struct hash_map_node;
typedef struct hash_map_node hash_map_node_t;

struct hash_map_list;
typedef struct hash_map_list hash_map_list_t;

typedef struct hash_map {
	size_t           size;
	size_t           table_size;
	hash_map_list_t* table;

	hash_map_hash_function    hash;
	hash_map_compare_function compare; 	
	hash_map_element_function destroy; 	

	#ifdef USE_ALLOCATORS
	alloc_function  alloc;
	free_function   free;
	#endif
} hash_map_t;

#ifdef USE_ALLOCATORS
boolean   hash_map_create      ( hash_map_t *p_map, size_t table_size, hash_map_hash_function hash_function, hash_map_element_function destroy, hash_map_compare_function compare, alloc_function alloc, free_function free );
#else
boolean   hash_map_create      ( hash_map_t *p_map, size_t table_size, hash_map_hash_function hash_function, hash_map_element_function destroy, hash_map_compare_function compare );
#endif
void      hash_map_destroy     ( hash_map_t *p_map );
boolean   hash_map_insert      ( hash_map_t *p_map, const void *key, const void *value );
boolean   hash_map_remove      ( hash_map_t *p_map, const void *key );
boolean   hash_map_find        ( const hash_map_t *p_map, const void *key, void **value );
void      hash_map_clear       ( hash_map_t *p_map );
boolean   hash_map_resize      ( hash_map_t *p_map, size_t new_size );
boolean   hash_map_rehash      ( hash_map_t *p_map, float load_factor );
boolean   hash_map_serialize   ( hash_map_t *p_map, size_t key_size, size_t value_size, FILE *file );
boolean   hash_map_unserialize ( hash_map_t *p_map, size_t key_size, size_t value_size, FILE *file );
#ifdef USE_ALLOCATORS
void      hash_map_alloc_set   ( hash_map_t *p_map, alloc_function alloc );
void      hash_map_free_set    ( hash_map_t *p_map, free_function free );
#endif

#define   hash_map_size(p_map)         ((p_map)->size)
#define   hash_map_table_size(p_map)   ((p_map)->table_size)
#define   hash_map_load_factor(p_map)  (hash_map_size(p_map) / ((float) hash_map_table_size(p_map)))


typedef struct hash_map_iter {
	const hash_map_t* map;
	size_t            index;
	hash_map_node_t*  current;
	void*             key;
	void*             value;
} hash_map_iterator_t;

void    hash_map_iterator      ( const hash_map_t* p_map, hash_map_iterator_t* iter );
boolean hash_map_iterator_next ( hash_map_iterator_t* iter );
void*   hash_map_iterator_key  ( hash_map_iterator_t* iter );
void*   hash_map_iterator_value( hash_map_iterator_t* iter );

#ifdef __cplusplus
}
#endif 
#endif /* _HASH_MAP_H_ */
