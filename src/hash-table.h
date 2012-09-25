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
#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_
#ifdef __cplusplus
extern "C" {
#endif 

#include <stddef.h>
#include "libcollections-config.h"
#include "types.h"
#include "slist.h"
#ifdef USE_ALLOCATORS
#include "alloc.h"
#endif

/* Each size is a prime number */
#ifndef HASH_TABLE_SIZE_SMALL 
#define HASH_TABLE_SIZE_SMALL         (37)
#endif
#ifndef HASH_TABLE_SIZE_MEDIUM 
#define HASH_TABLE_SIZE_MEDIUM        (1031)
#endif
#ifndef HASH_TABLE_SIZE_LARGE
#define HASH_TABLE_SIZE_LARGE         (5153)
#endif
#ifndef HASH_TABLE_SIZE_EXTRA_LARGE
#define HASH_TABLE_SIZE_EXTRA_LARGE   (7919)
#endif

#ifndef HASH_TABLE_DEFAULT_LOAD_FACTOR
#define HASH_TABLE_DEFAULT_LOAD_FACTOR     (0.7)
#endif

#ifndef HASH_TABLE_THRESHOLD
#define HASH_TABLE_THRESHOLD               (0.7)
#endif

	

typedef size_t  (*hash_table_hash_function)    ( const void *element );
typedef boolean (*hash_table_element_function) ( void *element );
typedef int     (*hash_table_compare_function) ( const void *left, const void *right );

typedef struct hash_table {
	size_t   size;
	size_t   table_size;
	slist_t* table;

	hash_table_hash_function    hash;
	hash_table_compare_function compare; 	
	hash_table_element_function destroy; 	

	#ifdef USE_ALLOCATORS
	alloc_function  alloc;
	free_function   free;
	#endif
} hash_table_t;

#ifdef USE_ALLOCATORS
boolean   hash_table_create      ( hash_table_t *p_table, size_t table_size, hash_table_hash_function hash_function, hash_table_element_function destroy_callback, hash_table_compare_function compare_callback, alloc_function alloc, free_function free );
#else
boolean   hash_table_create      ( hash_table_t *p_table, size_t table_size, hash_table_hash_function hash_function, hash_table_element_function destroy_callback, hash_table_compare_function compare_callback );
#endif
void      hash_table_destroy     ( hash_table_t *p_table );
boolean   hash_table_insert      ( hash_table_t *p_table, const void *data );
boolean   hash_table_remove      ( hash_table_t *p_table, const void *data );
boolean   hash_table_find        ( const hash_table_t *p_table, const void *data, void **found_data );
void      hash_table_clear       ( hash_table_t *p_table );
boolean   hash_table_resize      ( hash_table_t *p_table, size_t size );
boolean   hash_table_rehash      ( hash_table_t *p_table, double load_factor );
boolean   hash_table_serialize   ( hash_table_t *p_table, size_t element_size, FILE *file );
boolean   hash_table_unserialize ( hash_table_t *p_table, size_t element_size, FILE *file );

#define   hash_table_size(p_table)         ((p_table)->size)
#define   hash_table_table_size(p_table)   ((p_table)->table_size)
#define   hash_table_load_factor(p_table)  (hash_table_size(p_table) / ((double) hash_table_table_size(p_table)))

typedef struct hash_table_iter {
	const hash_table_t* table;
	size_t              index;
	slist_node_t*       current;
	void*               data;
} hash_table_iterator_t;

void    hash_table_iterator      ( const hash_table_t* p_table, hash_table_iterator_t* iter );
boolean hash_table_iterator_next ( hash_table_iterator_t* iter );
void*   hash_table_iterator_data ( hash_table_iterator_t* iter );

#ifdef __cplusplus
}
#endif 
#endif /* _HASH_TABLE_H_ */
