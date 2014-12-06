/*
 * Copyright (C) 2010-2014 by Joseph A. Marrero.  http://www.manvscode.com/
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
#ifndef _SLIST_H_
#define _SLIST_H_
#ifdef __cplusplus
extern "C" {
#endif 

#include <stddef.h>
#include <stdbool.h>
#include "alloc.h"

typedef bool (*slist_element_function)( void *element );

typedef struct slist_node {
	struct slist_node* next;	
	void* data;
} slist_node_t;

typedef struct slist {
	slist_node_t* head;
	size_t size;
	slist_element_function destroy;

	alloc_function  alloc;
	free_function   free;
} slist_t;

typedef slist_node_t* slist_iterator_t;


void    slist_create        ( slist_t *p_list, slist_element_function destroy_callback, alloc_function alloc, free_function free );
void    slist_destroy       ( slist_t *p_list );
bool    slist_insert_front  ( slist_t *p_list, const void *data ); /* O(1) */
bool    slist_remove_front  ( slist_t *p_list ); /* O(1) */
bool    slist_insert_next   ( slist_t *p_list, slist_node_t* p_front_node, const void *data ); /* O(1) */
bool    slist_remove_next   ( slist_t *p_list, slist_node_t* p_front_node ); /* O(1) */ 
void    slist_clear         ( slist_t *p_list ); /* O(N) */

void    slist_alloc_set     ( slist_t *p_list, alloc_function alloc );
void    slist_free_set      ( slist_t *p_list, free_function free );

slist_iterator_t slist_begin  ( const slist_t *p_list );
#define          slist_end( ) ((slist_iterator_t)NULL)
slist_iterator_t slist_next   ( const slist_iterator_t iter );

#define slist_push               slist_insert_front
#define slist_pop                slist_remove_front

#define slist_head(p_list)       ((p_list)->head)
#define slist_front(p_list)      ((p_list)->head)
#define slist_size(p_list)       ((p_list)->size)
#define slist_is_empty(p_list)   ((p_list)->size <= 0)

#ifdef __cplusplus
}
#endif 
#endif /* _SLIST_H_ */
