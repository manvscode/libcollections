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
#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "slist.h"
#include "alloc.h"

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


typedef size_t  (*lc_hash_table_hash_fxn_t)    ( const void *element );
typedef bool    (*lc_hash_table_element_fxn_t) ( void *element );
typedef int     (*lc_hash_table_compare_fxn_t) ( const void *left, const void *right );

typedef struct lc_hash_table {
	size_t   size;
	size_t   table_size;
	lc_slist_t* table;

	lc_hash_table_hash_fxn_t    hash;
	lc_hash_table_compare_fxn_t compare;
	lc_hash_table_element_fxn_t destroy;

	lc_alloc_fxn_t  alloc;
	lc_free_fxn_t   free;
} lc_hash_table_t;

bool      lc_hash_table_create      ( lc_hash_table_t* p_table, size_t table_size, lc_hash_table_hash_fxn_t hash_function, lc_hash_table_element_fxn_t destroy_callback, lc_hash_table_compare_fxn_t compare_callback, lc_alloc_fxn_t alloc, lc_free_fxn_t free );
void      lc_hash_table_destroy     ( lc_hash_table_t* p_table );
bool      lc_hash_table_insert      ( lc_hash_table_t* p_table, const void *data );
bool      lc_hash_table_remove      ( lc_hash_table_t* p_table, const void *data );
bool      lc_hash_table_find        ( const lc_hash_table_t* p_table, const void *data, void **found_data );
void      lc_hash_table_clear       ( lc_hash_table_t* p_table );
bool      lc_hash_table_resize      ( lc_hash_table_t* p_table, size_t size );
bool      lc_hash_table_rehash      ( lc_hash_table_t* p_table, double load_factor );
bool      lc_hash_table_serialize   ( lc_hash_table_t* p_table, size_t element_size, FILE *file );
bool      lc_hash_table_unserialize ( lc_hash_table_t* p_table, size_t element_size, FILE *file );

#define   lc_hash_table_size(p_table)         ((p_table)->size)
#define   lc_hash_table_table_size(p_table)   ((p_table)->table_size)
#define   lc_hash_table_load_factor(p_table)  (lc_hash_table_size(p_table) / ((double) lc_hash_table_table_size(p_table)))

typedef struct lc_hash_table_iter {
	const lc_hash_table_t* table;
	size_t              index;
	lc_slist_node_t*       current;
	void*               data;
} lc_hash_table_iterator_t;

void    lc_hash_table_iterator      ( const lc_hash_table_t* p_table, lc_hash_table_iterator_t* iter );
bool    lc_hash_table_iterator_next ( lc_hash_table_iterator_t* iter );
void*   lc_hash_table_iterator_data ( lc_hash_table_iterator_t* iter );

#ifdef __cplusplus
}
#endif 
#endif /* _HASH_TABLE_H_ */
