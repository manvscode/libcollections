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
#ifndef _LC_SLIST_H_
#define _LC_SLIST_H_
/**
 * @file slist.h
 * @brief A singly linked-list collection.
 *
 * @defgroup lc_slist Singly Linked-List
 * @ingroup Collections
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include "alloc.h"

typedef bool (*lc_slist_element_fxn_t)( void *element );

typedef struct lc_slist_node {
	struct lc_slist_node* next;
	void* data;
} lc_slist_node_t;

typedef struct lc_slist {
	lc_slist_node_t* head;
	size_t size;
	lc_slist_element_fxn_t destroy;

	lc_alloc_fxn_t  alloc;
	lc_free_fxn_t   free;
} lc_slist_t;

typedef lc_slist_node_t* lc_slist_iterator_t;


void    lc_slist_create        ( lc_slist_t *p_list, lc_slist_element_fxn_t destroy_callback, lc_alloc_fxn_t alloc, lc_free_fxn_t free );
void    lc_slist_destroy       ( lc_slist_t *p_list );
bool    lc_slist_insert_front  ( lc_slist_t *p_list, const void *data ); /* O(1) */
bool    lc_slist_remove_front  ( lc_slist_t *p_list ); /* O(1) */
bool    lc_slist_insert_next   ( lc_slist_t *p_list, lc_slist_node_t* p_front_node, const void *data ); /* O(1) */
bool    lc_slist_remove_next   ( lc_slist_t *p_list, lc_slist_node_t* p_front_node ); /* O(1) */
void    lc_slist_clear         ( lc_slist_t *p_list ); /* O(N) */

void    lc_slist_alloc_set     ( lc_slist_t *p_list, lc_alloc_fxn_t alloc );
void    lc_slist_free_set      ( lc_slist_t *p_list, lc_free_fxn_t free );

lc_slist_iterator_t lc_slist_begin  ( const lc_slist_t *p_list );
#define          lc_slist_end( ) ((lc_slist_iterator_t)NULL)
lc_slist_iterator_t lc_slist_next   ( const lc_slist_iterator_t iter );

#define lc_slist_push               lc_slist_insert_front
#define lc_slist_pop                lc_slist_remove_front

#define lc_slist_head(p_list)       ((p_list)->head)
#define lc_slist_front(p_list)      ((p_list)->head)
#define lc_slist_size(p_list)       ((p_list)->size)
#define lc_slist_is_empty(p_list)   ((p_list)->size <= 0)

#ifdef __cplusplus
}
#endif
#endif /* _LC_SLIST_H_ */
