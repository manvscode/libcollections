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
#ifndef _RBTREE_H_
#define _RBTREE_H_
#ifdef __cplusplus
extern "C" {
#endif 

//#include "libcollections-config.h"
#include "types.h"
#include "alloc.h"

typedef int     (*rbtree_compare_function) ( const void *p_data_left, const void *p_data_right );
typedef boolean (*rbtree_element_function) ( void *p_data );


typedef struct rbnode {   
	struct rbnode *parent;
	struct rbnode *left;
	struct rbnode *right;
	boolean is_red;
	void *data;
} rbnode_t;

#ifdef EXTERN_RBNIL
extern rbnode RBNIL;
#endif

typedef struct rbtree {
	rbnode_t* root;
	size_t    size;	
	rbtree_compare_function _compare;
	rbtree_element_function _destroy;

	alloc_function  _alloc;
	free_function   _free;
} rbtree_t;

typedef rbnode_t* rbtree_iterator_t;


rbtree_t* rbtree_create_ex   ( rbtree_element_function destroy, rbtree_compare_function compare, alloc_function alloc, free_function free );
void      rbtree_create      ( rbtree_t *p_tree, rbtree_element_function destroy, rbtree_compare_function compare, alloc_function alloc, free_function free );
void      rbtree_destroy     ( rbtree_t *p_tree );
void      rbtree_copy        ( rbtree_t const *p_srcTree, rbtree_t *p_dstTree );
boolean   rbtree_insert      ( rbtree_t *p_tree, const void *data );
boolean   rbtree_remove      ( rbtree_t *p_tree, const void *data );
boolean   rbtree_search      ( const rbtree_t *p_tree, const void *data );
void      rbtree_clear       ( rbtree_t *p_tree );
boolean   rbtree_serialize   ( rbtree_t *p_tree, size_t element_size, FILE *file );
boolean   rbtree_unserialize ( rbtree_t *p_tree, size_t element_size, FILE *file );

void    rbtree_alloc_set   ( rbtree_t *p_tree, alloc_function alloc );
void    rbtree_free_set    ( rbtree_t *p_tree, free_function free );

rbnode_t* rbnode_minimum     ( rbnode_t *t );
rbnode_t* rbnode_maximum     ( rbnode_t *t );
rbnode_t* rbnode_successor   ( const rbnode_t *t );
rbnode_t* rbnode_predecessor ( const rbnode_t *t );

rbtree_iterator_t rbtree_begin ( const rbtree_t *p_tree );
rbtree_iterator_t rbtree_end   ( );

#ifdef _DEBUG_RBTREE 
boolean rbtree_verify_tree ( rbtree_t *p_tree );
void    rbtree_print       ( const rbtree_t *p_tree );
#endif

#define rbtree_size( p_tree )        ((p_tree)->size)
#define rbtree_is_empty( p_tree )    ((p_tree)->size <= 0)
#define rbtree_next( p_node )        (rbnode_successor( (p_node) ))
#define rbtree_previous( p_node )    (rbnode_predecessor( (p_node) ))
#define rbtree_root( p_tree )        ((p_tree)->root)

#ifdef __cplusplus
}
#endif 
#endif /* _RBTREE_H_ */
