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
#ifndef _LHASH_TABLE_H_
#define _LHASH_TABLE_H_
#ifdef __cplusplus
extern "C" {
#endif 

#include <stddef.h>
#include "libcollections-config.h"
#include "types.h"
#include "array.h"
#include "bitset.h"

/*
 *    Open Addressing Hash Table
 */

/* Each size is a prime number */
#ifndef LHASH_TABLE_SIZE_SMALL 
#define LHASH_TABLE_SIZE_SMALL         (37)
#endif
#ifndef LHASH_TABLE_SIZE_MEDIUM 
#define LHASH_TABLE_SIZE_MEDIUM        (1031)
#endif
#ifndef LHASH_TABLE_SIZE_LARGE
#define LHASH_TABLE_SIZE_LARGE         (5153)
#endif
#ifndef LHASH_TABLE_SIZE_EXTRA_LARGE
#define LHASH_TABLE_SIZE_EXTRA_LARGE   (7919)
#endif

#ifndef LHASH_TABLE_LOAD_FACTOR
#define LHASH_TABLE_LOAD_FACTOR        (0.7)
#endif
#ifndef LHASH_TABLE_THRESHOLD
#define LHASH_TABLE_THRESHOLD          (0.1)
#endif
#ifndef LHASH_TABLE_LINEAR_CONSTANT
#define LHASH_TABLE_LINEAR_CONSTANT    (1)
#endif

typedef size_t  (*lhash_table_hash_function)    ( const void *element );
typedef boolean (*lhash_table_element_function) ( void *element );
typedef int     (*lhash_table_compare_function) ( const void *left, const void *right );

typedef struct lhash_table {
	array_t  table;
	bitset_t occupied;
	bitset_t deleted;
	size_t   size;

	lhash_table_hash_function    hash_callback;
	lhash_table_compare_function compare_callback; 	
} lhash_table_t;

boolean   lhash_table_create  ( lhash_table_t *p_table, size_t element_size, size_t table_size, 
                                lhash_table_hash_function hash_function, 
                                lhash_table_compare_function compare_function );
void      lhash_table_destroy ( lhash_table_t *p_table );
boolean   lhash_table_insert  ( lhash_table_t *p_table, const void *data );
boolean   lhash_table_remove  ( lhash_table_t *p_table, const void *data );
boolean   lhash_table_find    ( lhash_table_t *p_table, const void *data, void **found_data );
void      lhash_table_clear   ( lhash_table_t *p_table );
boolean   lhash_table_resize  ( lhash_table_t *p_table, size_t new_size );
boolean   lhash_table_rehash  ( lhash_table_t *p_table, double load_factor );


#define   lhash_table_size(p_table)         ((p_table)->size)
#define   lhash_table_table_size(p_table)   ((p_table)->table.size)
#define   lhash_table_load_factor(p_table)  (lhash_table_size(p_table) / ((double) ((p_table)->table.size)))




#ifdef __cplusplus
}
#endif 
#endif /* _LHASH_TABLE_H_ */
