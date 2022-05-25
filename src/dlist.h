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
#ifndef _LC_DLIST_H_
#define _LC_DLIST_H_
/**
 * @file dlist.h
 * @brief A doubly linked-list collection.
 *
 * @defgroup lc_dlist Doubly Linked-List
 * @ingroup Collections
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
//#include "libcollections-config.h"
#include "alloc.h"

/**
 * lc_dlist_element_fxn_t is a function pointer for the doubly
 * linked-list element callback functions. This is the signature
 * of any element destruction functions.
 */
typedef bool (*lc_dlist_element_fxn_t)( void *element );

/**
 * lc_dlist_node_t is a linked-list node object.
 */
typedef struct lc_dlist_node {
	void* data;
	struct lc_dlist_node* next;
	struct lc_dlist_node* prev;
} lc_dlist_node_t;

/**
 * lc_dlist_t is a linked-list collection object.
 */
typedef struct dlist {
	lc_dlist_node_t* head;
	lc_dlist_node_t* tail;
	size_t size;
	lc_dlist_element_fxn_t destroy;

	lc_alloc_fxn_t  alloc;
	lc_free_fxn_t   free;
} lc_dlist_t;

/**
 * lc_dlist_iterator_t is a linked-list collection
 * iterator object.
 */
typedef lc_dlist_node_t* lc_dlist_iterator_t;

/**
 * Create a doubly linked-list collection.
 */
void lc_dlist_create        ( lc_dlist_t* p_list, lc_dlist_element_fxn_t destroy_callback, lc_alloc_fxn_t alloc, lc_free_fxn_t free );
/**
 * Destroy a doubly linked-list collection.
 */
void lc_dlist_destroy       ( lc_dlist_t* p_list );
/**
 * Insert an item at the beginning of the collection.
 */
bool lc_dlist_insert_front  ( lc_dlist_t* p_list, const void *data ); /* O(1) */
/**
 * Remove an item at the beginning of the collection.
 */
bool lc_dlist_remove_front  ( lc_dlist_t* p_list ); /* O(1) */
/**
 * Insert an item at the end of the collection.
 */
bool lc_dlist_insert_back   ( lc_dlist_t* p_list, const void *data ); /* O(1) */
/**
 * Remove an item at the end of the collection.
 */
bool lc_dlist_remove_back   ( lc_dlist_t* p_list ); /* O(1) */
/**
 * Insert an item before another item in the collection.
 */
bool lc_dlist_insert_next   ( lc_dlist_t* p_list, lc_dlist_node_t* p_front_node, const void *data ); /* O(1) */
/**
 * Remove an item that resides before another item in the collection.
 */
bool lc_dlist_remove_next   ( lc_dlist_t* p_list, lc_dlist_node_t* p_front_node ); /* O(1) */
/**
 * Empty the collection.
 */
void lc_dlist_clear         ( lc_dlist_t* p_list ); /* O(N) */

/**
 * Set the allocation callback that should be used for allocating
 * memory.
 */
void lc_dlist_alloc_set  ( lc_dlist_t* p_list, lc_alloc_fxn_t alloc );
/**
 * Set the deallocation callback that should be used for releasing
 * memory.
 */
void lc_dlist_free_set   ( lc_dlist_t* p_list, lc_free_fxn_t free );

/**
 * Get an iterator for iterating over the collection.
 */
lc_dlist_iterator_t lc_dlist_begin    ( const lc_dlist_t* p_list );
/**
 * Get a reverse iterator for iterating over the collection.
 */
lc_dlist_iterator_t lc_dlist_rbegin   ( const lc_dlist_t* p_list );
/**
 * Get an iterator for the end.
 */
#define             lc_dlist_end( )   ((lc_dlist_iterator_t)NULL)
/**
 * Move the iterator to the next item.
 */
lc_dlist_iterator_t lc_dlist_next     ( const lc_dlist_iterator_t iter );
/**
 * Move the iterator to the previous item.
 */
lc_dlist_iterator_t lc_dlist_previous ( const lc_dlist_iterator_t iter );

/**
 * Push an item into the collection.
 */
#define lc_dlist_push               lc_dlist_insert_back
/**
 * Pop an item off the collection.
 */
#define lc_dlist_pop                lc_dlist_remove_front

#define lc_dlist_head(p_list)       ((p_list)->head)
#define lc_dlist_front(p_list)      ((p_list)->head)
#define lc_dlist_tail(p_list)       ((p_list)->tail)
#define lc_dlist_back(p_list)       ((p_list)->tail)
#define lc_dlist_size(p_list)       ((p_list)->size)
#define lc_dlist_is_empty(p_list)   ((p_list)->size <= 0)

#ifdef __cplusplus
}
#endif
#endif /* _LC_DLIST_H_ */
