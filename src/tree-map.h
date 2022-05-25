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
#ifndef _LC_TREE_MAP_H_
#define _LC_TREE_MAP_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "alloc.h"

typedef int     (*lc_tree_map_compare_fxn_t) ( const void *p_key_left, const void *p_key_right );
typedef bool    (*lc_tree_map_element_fxn_t) ( void *p_key, void *p_value );



typedef struct lc_lc_tree_map_node {
	struct lc_lc_tree_map_node* parent;
	struct lc_lc_tree_map_node* left;
	struct lc_lc_tree_map_node* right;
	bool  is_red;
	void* key;
	void* value;
} lc_tree_map_node_t;

typedef struct lc_tree_map {
	lc_tree_map_node_t *root;
	size_t size;
	lc_tree_map_compare_fxn_t compare;
	lc_tree_map_element_fxn_t destroy;

	lc_alloc_fxn_t  _alloc;
	lc_free_fxn_t   _free;
} lc_tree_map_t;

typedef lc_tree_map_node_t* lc_tree_map_iterator_t;


lc_tree_map_t* lc_tree_map_create_ex   ( lc_tree_map_element_fxn_t destroy, lc_tree_map_compare_fxn_t compare, lc_alloc_fxn_t alloc, lc_free_fxn_t free );
void        lc_tree_map_create      ( lc_tree_map_t *p_map, lc_tree_map_element_fxn_t destroy, lc_tree_map_compare_fxn_t compare, lc_alloc_fxn_t alloc, lc_free_fxn_t free );
void        lc_tree_map_destroy     ( lc_tree_map_t *p_map );
void        lc_tree_map_copy        ( lc_tree_map_t const *p_srcTree, lc_tree_map_t *p_dstTree );
bool        lc_tree_map_insert      ( lc_tree_map_t *p_map, const void *key, const void *value );
bool        lc_tree_map_remove      ( lc_tree_map_t *p_map, const void *key );
bool        lc_tree_map_search      ( const lc_tree_map_t *p_map, const void *key, void **value ); /* deprecated */
void        lc_tree_map_clear       ( lc_tree_map_t *p_map );
bool        lc_tree_map_serialize   ( lc_tree_map_t *p_map, size_t key_size, size_t value_size, FILE *file );
bool        lc_tree_map_unserialize ( lc_tree_map_t *p_map, size_t key_size, size_t value_size, FILE *file );
void        lc_tree_map_alloc_set   ( lc_tree_map_t *p_map, lc_alloc_fxn_t alloc );
void        lc_tree_map_free_set    ( lc_tree_map_t *p_map, lc_free_fxn_t free );

lc_tree_map_node_t* lc_tree_map_node_minimum     ( lc_tree_map_node_t *t );
lc_tree_map_node_t* lc_tree_map_node_maximum     ( lc_tree_map_node_t *t );
lc_tree_map_node_t* lc_tree_map_node_successor   ( lc_tree_map_node_t *t );
lc_tree_map_node_t* lc_tree_map_node_predecessor ( lc_tree_map_node_t *t );

lc_tree_map_iterator_t lc_tree_map_begin ( const lc_tree_map_t *p_map );
lc_tree_map_iterator_t lc_tree_map_end   ( );
lc_tree_map_iterator_t lc_tree_map_find  ( const lc_tree_map_t *p_map, const void *key );


#ifdef LC_DEBUG_TREE_MAP
bool    lc_tree_map_verify_tree ( lc_tree_map_t *p_map );
void    lc_tree_map_print       ( const lc_tree_map_t *p_map );
#endif

#define lc_tree_map_size( p_map )        ((p_map)->size)
#define lc_tree_map_is_empty( p_map )    ((p_map)->size <= 0)
#define lc_tree_map_next( p_node )       (lc_tree_map_node_successor( (p_node) ))
#define lc_tree_map_previous( p_node )   (lc_tree_map_node_predecessor( (p_node) ))
#define lc_tree_map_root( p_map )        ((p_map)->root)

#ifdef __cplusplus
}
#endif
#endif /* _LC_TREE_MAP_H_ */
