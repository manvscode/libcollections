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
#ifndef _LC_LHASH_TABLE_H_
#define _LC_LHASH_TABLE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include "array.h"
#include "bitset.h"
#include "alloc.h"

/*
 *    Open Addressing Hash Table
 */

/* Each size is a prime number */
#ifndef LC_LHASH_TABLE_SIZE_SMALL
#define LC_LHASH_TABLE_SIZE_SMALL         (37)
#endif
#ifndef LC_LHASH_TABLE_SIZE_MEDIUM
#define LC_LHASH_TABLE_SIZE_MEDIUM        (1031)
#endif
#ifndef LC_LHASH_TABLE_SIZE_LARGE
#define LC_LHASH_TABLE_SIZE_LARGE         (5153)
#endif
#ifndef LC_LHASH_TABLE_SIZE_EXTRA_LARGE
#define LC_LHASH_TABLE_SIZE_EXTRA_LARGE   (7919)
#endif

#ifndef LC_LHASH_TABLE_LOAD_FACTOR
#define LC_LHASH_TABLE_LOAD_FACTOR        (0.7)
#endif
#ifndef LC_LHASH_TABLE_THRESHOLD
#define LC_LHASH_TABLE_THRESHOLD          (0.1)
#endif
#ifndef LC_LHASH_TABLE_LINEAR_CONSTANT
#define LC_LHASH_TABLE_LINEAR_CONSTANT    (1)
#endif

typedef size_t  (*lc_lhash_table_hash_fxn_t)    ( const void *element );
typedef bool    (*lc_lhash_table_element_fxn_t) ( void *element );
typedef int     (*lc_lhash_table_compare_fxn_t) ( const void *left, const void *right );

typedef struct lc_lhash_table {
	lc_alloc_fxn_t  alloc;
	lc_free_fxn_t   free;

	lc_array_t  table;
	lc_bitset_t occupied;
	lc_bitset_t deleted;
	size_t   size;

	lc_lhash_table_hash_fxn_t    hash_callback;
	lc_lhash_table_compare_fxn_t compare_callback;
} lc_lhash_table_t;

bool      lc_lhash_table_create  ( lc_lhash_table_t* p_table, size_t element_size, size_t table_size,
                                lc_lhash_table_hash_fxn_t hash_function,
                                lc_lhash_table_compare_fxn_t compare_function,
								lc_alloc_fxn_t alloc,
								lc_free_fxn_t free );
void      lc_lhash_table_destroy ( lc_lhash_table_t* p_table );
bool      lc_lhash_table_insert  ( lc_lhash_table_t* p_table, const void *data );
bool      lc_lhash_table_remove  ( lc_lhash_table_t* p_table, const void *data );
bool      lc_lhash_table_find    ( lc_lhash_table_t* p_table, const void *data, void **found_data );
void      lc_lhash_table_clear   ( lc_lhash_table_t* p_table );
bool      lc_lhash_table_resize  ( lc_lhash_table_t* p_table, size_t new_size );
bool      lc_lhash_table_rehash  ( lc_lhash_table_t* p_table, double load_factor );


#define   lc_lhash_table_size(p_table)         ((p_table)->size)
#define   lc_lhash_table_table_size(p_table)   ((p_table)->table.size)
#define   lc_lhash_table_load_factor(p_table)  (lc_lhash_table_size(p_table) / ((double) ((p_table)->table.size)))

#ifdef __cplusplus
}
#endif
#endif /* _LC_LHASH_TABLE_H_ */
