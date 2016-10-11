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
#ifndef _LC_RBTREE_H_
#define _LC_RBTREE_H_
/**
 * @file rbtree.h
 * @brief A red-black tree collection.
 *
 * A red-black tree is a binary tree that remains roughly balanced.
 *
 * @defgroup lc_rbtree Red-Black Tree
 * @ingroup Collections
 *
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
//#include "libcollections-config.h"
#include "alloc.h"

typedef int     (*lc_rbtree_compare_fxn_t) ( const void *p_data_left, const void *p_data_right );
typedef bool    (*lc_rbtree_element_fxn_t) ( void *p_data );


typedef struct lc_rbnode {
	struct lc_rbnode *parent;
	struct lc_rbnode *left;
	struct lc_rbnode *right;
	bool   is_red;
	void *data;
} lc_rbnode_t;

#ifdef LC_EXTERN_RBNIL
extern lc_rbnode RBNIL;
#endif

typedef struct lc_rbtree {
	lc_rbnode_t* root;
	size_t    size;
	lc_rbtree_compare_fxn_t _compare;
	lc_rbtree_element_fxn_t _destroy;

	lc_alloc_fxn_t  _alloc;
	lc_free_fxn_t   _free;
} lc_rbtree_t;

typedef lc_rbnode_t* lc_rbtree_iterator_t;


lc_rbtree_t* lc_rbtree_create_ex   ( lc_rbtree_element_fxn_t destroy, lc_rbtree_compare_fxn_t compare, lc_alloc_fxn_t alloc, lc_free_fxn_t free );
void      lc_rbtree_create      ( lc_rbtree_t* p_tree, lc_rbtree_element_fxn_t destroy, lc_rbtree_compare_fxn_t compare, lc_alloc_fxn_t alloc, lc_free_fxn_t free );
void      lc_rbtree_destroy     ( lc_rbtree_t* p_tree );
void      lc_rbtree_copy        ( lc_rbtree_t const *p_srcTree, lc_rbtree_t* p_dstTree );
bool      lc_rbtree_insert      ( lc_rbtree_t* p_tree, const void *data );
bool      lc_rbtree_remove      ( lc_rbtree_t* p_tree, const void *data );
bool      lc_rbtree_search      ( const lc_rbtree_t* p_tree, const void *data );
void      lc_rbtree_clear       ( lc_rbtree_t* p_tree );
bool      lc_rbtree_serialize   ( lc_rbtree_t* p_tree, size_t element_size, FILE *file );
bool      lc_rbtree_unserialize ( lc_rbtree_t* p_tree, size_t element_size, FILE *file );

void    lc_rbtree_alloc_set   ( lc_rbtree_t* p_tree, lc_alloc_fxn_t alloc );
void    lc_rbtree_free_set    ( lc_rbtree_t* p_tree, lc_free_fxn_t free );

lc_rbnode_t* lc_rbnode_minimum     ( lc_rbnode_t *t );
lc_rbnode_t* lc_rbnode_maximum     ( lc_rbnode_t *t );
lc_rbnode_t* lc_rbnode_successor   ( const lc_rbnode_t *t );
lc_rbnode_t* lc_rbnode_predecessor ( const lc_rbnode_t *t );

lc_rbtree_iterator_t lc_rbtree_begin ( const lc_rbtree_t* p_tree );
lc_rbtree_iterator_t lc_rbtree_end   ( );

#ifdef LC_RBTREE_DEBUG
bool    lc_rbtree_verify_tree ( lc_rbtree_t* p_tree );
void    lc_rbtree_print       ( const lc_rbtree_t* p_tree );
#endif

#define lc_rbtree_size( p_tree )        ((p_tree)->size)
#define lc_rbtree_is_empty( p_tree )    ((p_tree)->size <= 0)
#define lc_rbtree_next( p_node )        (lc_rbnode_successor( (p_node) ))
#define lc_rbtree_previous( p_node )    (lc_rbnode_predecessor( (p_node) ))
#define lc_rbtree_root( p_tree )        ((p_tree)->root)

#ifdef __cplusplus
}
#endif
#endif /* _LC_RBTREE_H_ */
