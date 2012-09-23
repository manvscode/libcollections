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
#ifndef _TREE_MAP_H_
#define _TREE_MAP_H_
#ifdef __cplusplus
extern "C" {
#endif 

#include "types.h"
#ifdef USE_ALLOCATORS
#include "alloc.h"
#endif

typedef int     (*tree_map_compare_function) ( const void *p_key_left, const void *p_key_right );
typedef boolean (*tree_map_element_function) ( void *p_key, void *p_value );



typedef struct tree_map_node {   
	struct tree_map_node *parent;
	struct tree_map_node *left;
	struct tree_map_node *right;
	boolean is_red;
	void *key;
	void *value;
} tree_map_node_t;

typedef struct tree_map {
	tree_map_node_t *root;
	size_t size;	
	tree_map_compare_function compare;
	tree_map_element_function destroy;

	#ifdef USE_ALLOCATORS
	alloc_function  _alloc;
	free_function   _free;
	#endif
} tree_map_t;

typedef tree_map_node_t* tree_map_iterator_t;


#ifdef USE_ALLOCATORS
tree_map_t* tree_map_create_ex   ( tree_map_element_function destroy, tree_map_compare_function compare, alloc_function alloc, free_function free );
void        tree_map_create      ( tree_map_t *p_map, tree_map_element_function destroy, tree_map_compare_function compare, alloc_function alloc, free_function free );
#else
tree_map_t* tree_map_create_ex   ( tree_map_element_function destroy, tree_map_compare_function compare );
void        tree_map_create      ( tree_map_t *p_map, tree_map_element_function destroy, tree_map_compare_function compare );
#endif
void        tree_map_destroy     ( tree_map_t *p_map );
void        tree_map_copy        ( tree_map_t const *p_srcTree, tree_map_t *p_dstTree );
boolean     tree_map_insert      ( tree_map_t *p_map, const void *key, const void *value );
boolean     tree_map_remove      ( tree_map_t *p_map, const void *key );
boolean     tree_map_find        ( const tree_map_t *p_map, const void *key, void **value );
void        tree_map_clear       ( tree_map_t *p_map );
boolean     tree_map_serialize   ( tree_map_t *p_map, size_t key_size, size_t value_size, FILE *file );
boolean     tree_map_unserialize ( tree_map_t *p_map, size_t key_size, size_t value_size, FILE *file );
#ifdef USE_ALLOCATORS
void        tree_map_alloc_set   ( tree_map_t *p_map, alloc_function alloc );
void        tree_map_free_set    ( tree_map_t *p_map, free_function free );
#endif

tree_map_node_t* tree_map_node_minimum     ( tree_map_node_t *t );
tree_map_node_t* tree_map_node_maximum     ( tree_map_node_t *t );
tree_map_node_t* tree_map_node_successor   ( tree_map_node_t *t );
tree_map_node_t* tree_map_node_predecessor ( tree_map_node_t *t );

tree_map_iterator_t tree_map_begin ( const tree_map_t *p_map );
tree_map_iterator_t tree_map_end   ( );


#ifdef _DEBUG_TREE_MAP 
boolean tree_map_verify_tree ( tree_map_t *p_map );
void    tree_map_print       ( const tree_map_t *p_map );
#endif

#define tree_map_size( p_map )        ((p_map)->size)
#define tree_map_is_empty( p_map )    ((p_map)->size <= 0)
#define tree_map_next( p_node )       (tree_map_node_successor( (p_node) ))
#define tree_map_previous( p_node )   (tree_map_node_predecessor( (p_node) ))
#define tree_map_root( p_map )        ((p_map)->root)

#ifdef __cplusplus
}
#endif 
#endif /* _TREE_MAP_H_ */
