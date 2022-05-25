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
#ifndef _LC_HASH_MAP_H_
#define _LC_HASH_MAP_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include "alloc.h"

/* Each size is a prime number */
#ifndef LC_HASH_MAP_SIZE_SMALL
#define LC_HASH_MAP_SIZE_SMALL         (101)
#endif
#ifndef LC_HASH_MAP_SIZE_MEDIUM
#define LC_HASH_MAP_SIZE_MEDIUM        (1031)
#endif
#ifndef LC_HASH_MAP_SIZE_LARGE
#define LC_HASH_MAP_SIZE_LARGE         (5153)
#endif
#ifndef LC_HASH_MAP_SIZE_EXTRA_LARGE
#define LC_HASH_MAP_SIZE_EXTRA_LARGE   (7919)
#endif

#ifndef LC_HASH_MAP_DEFAULT_LOAD_FACTOR
#define LC_HASH_MAP_DEFAULT_LOAD_FACTOR     (0.7)
#endif

#ifndef LC_HASH_MAP_THRESHOLD
#define LC_HASH_MAP_THRESHOLD               (0.7)
#endif



typedef size_t  (*lc_hash_map_hash_fxn_t)    ( const void *key );
typedef bool    (*lc_hash_map_element_fxn_t) ( void *key, void *value );
typedef int     (*lc_hash_map_compare_fxn_t) ( const void* __restrict left, const void* __restrict right );

struct lc_hash_map_node;
typedef struct lc_hash_map_node lc_hash_map_node_t;

struct lc_hash_map_list;
typedef struct lc_hash_map_list lc_hash_map_list_t;

typedef struct lc_hash_map {
	size_t           size;
	size_t           table_size;
	lc_hash_map_list_t* table;

	lc_hash_map_hash_fxn_t    hash;
	lc_hash_map_compare_fxn_t compare;
	lc_hash_map_element_fxn_t destroy;

	lc_alloc_fxn_t  alloc;
	lc_free_fxn_t   free;
} lc_hash_map_t;

bool      lc_hash_map_create      ( lc_hash_map_t *p_map, size_t table_size,
                                 lc_hash_map_hash_fxn_t hash_fxn_t, lc_hash_map_element_fxn_t destroy,
                                 lc_hash_map_compare_fxn_t compare,
                                 lc_alloc_fxn_t alloc, lc_free_fxn_t free );
void      lc_hash_map_destroy     ( lc_hash_map_t* p_map );
bool      lc_hash_map_insert      ( lc_hash_map_t* __restrict p_map, const void* __restrict key, const void* __restrict value );
bool      lc_hash_map_remove      ( lc_hash_map_t* __restrict p_map, const void* __restrict key );
bool      lc_hash_map_find        ( const lc_hash_map_t* __restrict p_map, const void* __restrict key, void ** __restrict value );
void      lc_hash_map_clear       ( lc_hash_map_t *p_map );
bool      lc_hash_map_resize      ( lc_hash_map_t *p_map, size_t new_size );
bool      lc_hash_map_rehash      ( lc_hash_map_t *p_map, float load_factor );
bool      lc_hash_map_serialize   ( lc_hash_map_t *p_map, size_t key_size, size_t value_size, FILE *file );
bool      lc_hash_map_unserialize ( lc_hash_map_t *p_map, size_t key_size, size_t value_size, FILE *file );
void      lc_hash_map_alloc_set   ( lc_hash_map_t *p_map, lc_alloc_fxn_t alloc );
void      lc_hash_map_free_set    ( lc_hash_map_t *p_map, lc_free_fxn_t free );

#define   lc_hash_map_size(p_map)         ((p_map)->size)
#define   lc_hash_map_table_size(p_map)   ((p_map)->table_size)
#define   lc_hash_map_load_factor(p_map)  (lc_hash_map_size(p_map) / ((float) lc_hash_map_table_size(p_map)))


typedef struct lc_hash_map_iter {
	const lc_hash_map_t* map;
	size_t            index;
	lc_hash_map_node_t*  current;
	void*             key;
	void*             value;
} lc_hash_map_iterator_t;

void    lc_hash_map_iterator      ( const lc_hash_map_t* p_map, lc_hash_map_iterator_t* iter );
bool    lc_hash_map_iterator_next ( lc_hash_map_iterator_t* iter );
void*   lc_hash_map_iterator_key  ( lc_hash_map_iterator_t* iter );
void*   lc_hash_map_iterator_value( lc_hash_map_iterator_t* iter );

#ifdef __cplusplus
}
#endif
#endif /* _LC_HASH_MAP_H_ */
