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
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "rbtree.h"

#ifdef EXTERN_RBNIL
	/* Typical leaf node (always black).
	 * This cannot be const because the parent pointer is manipulated.
	 */
	lc_rbnode_t RBNIL = { (lc_rbnode_t *) &RBNIL, (lc_rbnode_t *) &RBNIL, (lc_rbnode_t *) &RBNIL, false, NULL };
#else
	/* Typical leaf node (always black) */
	static lc_rbnode_t RBNIL = { (lc_rbnode_t *) &RBNIL, (lc_rbnode_t *) &RBNIL, (lc_rbnode_t *) &RBNIL, false, NULL };
#endif

#if defined(RBTREE_DESTROY_CHECK) || defined(DESTROY_CHECK_ALL)
	#define DESTROY_CHECK( code ) \
		if( p_tree->_destroy ) \
		{ \
			code \
		}
#else
	#define DESTROY_CHECK( code ) \
		code
#endif


#define nil_init( p_node ) \
	(p_node)->parent = p_node; \
	(p_node)->left   = p_node; \
	(p_node)->right  = p_node; \
	(p_node)->data   = NULL; \
	(p_node)->is_red = false;

#define lc_rbnode_init( p_node, p_data, p_parent, p_left, p_right, color ) \
	(p_node)->parent = p_parent; \
	(p_node)->left   = p_left; \
	(p_node)->right  = p_right; \
	(p_node)->data   = (void *) p_data; \
	(p_node)->is_red = color;


static lc_rbnode_t* lc_rbnode_search       ( lc_rbtree_t* p_tree, const void *key );


static inline void lc_rbtree_left_rotate( lc_rbtree_t* p_tree, lc_rbnode_t *x )
{
	lc_rbnode_t *y;
	assert( x->right != &RBNIL );

	y        = x->right;
	x->right = y->left;

	if( y->left != &RBNIL )
	{
		y->left->parent = x;
	}

	y->parent = x->parent;

	if( x->parent == &RBNIL )
	{
		p_tree->root = y;
	}
	else if( x == x->parent->left )
	{
		x->parent->left = y;
	}
	else
	{
		x->parent->right = y;
	}

	y->left   = x;
	x->parent = y;
}

static inline void lc_rbtree_right_rotate( lc_rbtree_t* p_tree, lc_rbnode_t *x )
{
	lc_rbnode_t *y;
	assert( x->left != &RBNIL );

	y       = x->left;
	x->left = y->right;

	if( y->right != &RBNIL )
	{
		y->right->parent = x;
	}

	y->parent = x->parent;

	if( x->parent == &RBNIL )
	{
		p_tree->root = y;
	}
	else if( x == x->parent->right ) {
		x->parent->right = y;
	}
	else
	{
		x->parent->left = y;
	}

	y->right  = x;
	x->parent = y;
}

static inline void lc_rbtree_insert_fixup( lc_rbtree_t* p_tree, lc_rbnode_t ** t )
{
	lc_rbnode_t *y = (lc_rbnode_t *) &RBNIL;

	while( (*t)->parent->is_red )
	{
		if( (*t)->parent == (*t)->parent->parent->left ) /* is parent on left side of grandparent... */
		{
			y = (*t)->parent->parent->right; /* y is uncle */
			/* case 1 */
			if( y->is_red ) /* uncle is red, just recolor */
			{
				(*t)->parent->is_red = false;
				y->is_red = false;
				(*t)->parent->parent->is_red = true;
				(*t) = (*t)->parent->parent;
			}
			else
			{
				/* case 2 - uncle is black and t is on the right */
				if( (*t) == (*t)->parent->right )
				{
					(*t) = (*t)->parent;
					lc_rbtree_left_rotate( p_tree, (*t) );
				}
				/* case 3 - uncle is black and t is on the left */
				(*t)->parent->is_red = false;
				(*t)->parent->parent->is_red = true;
				lc_rbtree_right_rotate( p_tree, (*t)->parent->parent );
			}
		}
		else  /* parent is on right side of grandparent... */
		{
			y = (*t)->parent->parent->left; /* y is uncle */
			/* case 1 */
			if( y->is_red ) /* uncle is red */
			{
				(*t)->parent->is_red = false;
				y->is_red = false;
				(*t)->parent->parent->is_red = true;
				(*t) = (*t)->parent->parent;
			}
			else
			{
				/* case 2 - uncle is black and t is on the left */
				if( (*t) == (*t)->parent->left )
				{
					(*t) = (*t)->parent;
					lc_rbtree_right_rotate( p_tree, (*t) );
				}
				/* case 3 - uncle is black and t is on the right */
				(*t)->parent->is_red = false;
				(*t)->parent->parent->is_red = true;
				lc_rbtree_left_rotate( p_tree, (*t)->parent->parent );
			}
		}
	}

	p_tree->root->is_red = false;
}

static inline void lc_rbtree_delete_fixup( lc_rbtree_t* p_tree, lc_rbnode_t ** t )
{
	lc_rbnode_t *w = (lc_rbnode_t *) &RBNIL;

	while( (*t) != p_tree->root && (*t)->is_red == false )
	{
		if( (*t) == (*t)->parent->left )
		{
			w = (*t)->parent->right;

			if( w->is_red )
			{
				w->is_red            = false;
				(*t)->parent->is_red = true;
				lc_rbtree_left_rotate( p_tree, (*t)->parent );
				w = (*t)->parent->right;
			}

			if( w->left->is_red == false && w->right->is_red == false )
			{
				w->is_red = true;
				(*t) = (*t)->parent;
			}
			else {
				if( w->right->is_red == false )
				{
					w->left->is_red = false;
					w->is_red       = true;
					lc_rbtree_right_rotate( p_tree, w );
					w = (*t)->parent->right;
				}

				w->is_red            = (*t)->parent->is_red;
				(*t)->parent->is_red = false;
				w->right->is_red     = false;
				lc_rbtree_left_rotate( p_tree, (*t)->parent );
				(*t) = p_tree->root;
			}
		}
		else
		{
			w = (*t)->parent->left;

			if( w->is_red )
			{
				w->is_red = false;
				(*t)->parent->is_red = true;
				lc_rbtree_right_rotate( p_tree, (*t)->parent );
				w = (*t)->parent->left;
			}

			if( w->right->is_red == false && w->left->is_red == false )
			{
				w->is_red = true;
				(*t) = (*t)->parent;
			}
			else
			{
				if( w->left->is_red == false )
				{
					w->right->is_red = false;
					w->is_red = true;
					lc_rbtree_left_rotate( p_tree, w );
					w = (*t)->parent->left;
				}

				w->is_red = (*t)->parent->is_red;
				(*t)->parent->is_red = false;
				w->left->is_red = false;
				lc_rbtree_right_rotate( p_tree, (*t)->parent );
				(*t) = p_tree->root;
			}
		}
	}

	(*t)->is_red = false;
}

lc_rbnode_t *lc_rbnode_minimum( lc_rbnode_t *t )
{
	while( t->left != &RBNIL ) { t = t->left; }
	return t;
}

lc_rbnode_t *lc_rbnode_maximum( lc_rbnode_t *t )
{
	while( t->right != &RBNIL ) { t = t->right; }
	return t;
}

lc_rbnode_t *lc_rbnode_successor( const lc_rbnode_t *t )
{
	lc_rbnode_t *y;

	if( t->right != &RBNIL )
	{
		return lc_rbnode_minimum( t->right );
	}

	y = t->parent;

	while( y != &RBNIL && t == y->right )
	{
		t = y;
		y = y->parent;
	}

	return y;
}

lc_rbnode_t *lc_rbnode_predecessor( const lc_rbnode_t *t )
{
	lc_rbnode_t *y;

	if( t->left != &RBNIL )
	{
		return lc_rbnode_maximum( t->left );
	}

	y = t->parent;

	while( y != &RBNIL && t == y->left )
	{
		t = y;
		y = y->parent;
	}

	return y;
}

lc_rbtree_t* lc_rbtree_create_ex( lc_rbtree_element_fxn_t destroy, lc_rbtree_compare_fxn_t compare, lc_alloc_fxn_t alloc, lc_free_fxn_t free )
{
	lc_rbtree_t* p_tree = (lc_rbtree_t*)  malloc( sizeof(lc_rbtree_t) );

	if( p_tree )
	{
		lc_rbtree_create( p_tree, destroy, compare, alloc, free );
	}

	return p_tree;
}

void lc_rbtree_create( lc_rbtree_t* p_tree, lc_rbtree_element_fxn_t destroy, lc_rbtree_compare_fxn_t compare, lc_alloc_fxn_t alloc, lc_free_fxn_t free )
{
	assert( p_tree );
	#ifndef RBTREE_CHECK_FOR_DESTROY
	assert( destroy );
	#endif
	assert( compare );
	p_tree->root     = (lc_rbnode_t *) &RBNIL;
	p_tree->size     = 0;
	p_tree->_compare = compare;
	p_tree->_destroy = destroy;


	p_tree->_alloc = malloc;
	p_tree->_free  = free;
}

void lc_rbtree_destroy( lc_rbtree_t* p_tree )
{
	assert( p_tree );
	lc_rbtree_clear( p_tree );

	#ifdef _DEBUG_RBTREE
	p_tree->root     = NULL;
	p_tree->size     = 0;
	p_tree->_compare = NULL;
	p_tree->_destroy = NULL;
	#endif
}

/*
void lc_rbtree_copy( lc_rbtree_t const *p_srcTree, lc_rbtree_t* p_dstTree )
{
	lc_rbnode_t *p_node = NULL;

	if( p_srcTree != p_dstTree )
	{
		lc_rbtree_clear( p_dstTree );

		p_dstTree->_compare = p_srcTree->compare;
		p_dstTree->_destroy = p_srcTree->destroy;

		// insert all of the nodes into this tree; m_NumberOfNodes should be equal after this...
		for( p_node = lc_rbtree_minimum(p_srcTree); p_node != &RBNIL; p_node = lc_rbnode_successor(p_node) )
		{
			lc_rbtree_insert( p_dstTree, p_node->data );
		}
	}
}
*/

bool lc_rbtree_insert( lc_rbtree_t* p_tree, const void *key )
{

	lc_rbnode_t *y;
	lc_rbnode_t *x;
	lc_rbnode_t *newNode;

	assert( p_tree );
	y   = (lc_rbnode_t *) &RBNIL;
	x   = p_tree->root;
	newNode = p_tree->_alloc( sizeof(lc_rbnode_t) );
	if( !newNode ) return false;

	/* Find where to insert the new node--y points the parent. */
	while( x != &RBNIL )
	{
		y = x;
		if( p_tree->_compare( key, x->data ) < 0 )
		{
			x = x->left;
		}
		else
		{
			x = x->right;
		}
	}

	/* Insert the new node. */
	if( y == &RBNIL )
	{
		p_tree->root = newNode;
	}
	else
	{
		if( p_tree->_compare( key, y->data ) < 0 )
		{
			y->left = newNode;
		}
		else
		{
			y->right = newNode;
		}
	}

	lc_rbnode_init( newNode, key, y, (lc_rbnode_t *) &RBNIL, (lc_rbnode_t *) &RBNIL, true );
	lc_rbtree_insert_fixup( p_tree, &newNode );
	p_tree->size++;

	return true;
}

bool lc_rbtree_remove( lc_rbtree_t* p_tree, const void *key )
{
	lc_rbnode_t *t;
	lc_rbnode_t *x;
	lc_rbnode_t *y;

	bool y_is_red = false;

	assert( p_tree );

	t = lc_rbnode_search( p_tree, key );

	if( t == NULL ) return false; /* item is not even in the tree! */

	if( t->left == &RBNIL || t->right == &RBNIL )
	{
		/* t has less than two children and can be deleted directly. */
		y = t;
	}
	else  /* t has two children... */
	{
		/* t has two children and must be switched out with it's
		 * successor. We will free the successor node and t's data.
		 */
		y = lc_rbnode_successor( t );
	}

	if( y->left != &RBNIL )
	{
		x = y->left;
	}
	else
	{
		x = y->right;
	}

	x->parent = y->parent;

	if( y->parent == &RBNIL )
	{
		p_tree->root = x;
	}
	else
	{
		if( y == y->parent->left )
		{
			y->parent->left = x;
		}
		else
		{
			y->parent->right = x;
		}
	}

	if( y != t )
	{
		DESTROY_CHECK(
			p_tree->_destroy( t->data );
		);

		t->data = y->data;
		y_is_red = y->is_red;
		p_tree->_free( y );
	}
	else
	{
		DESTROY_CHECK(
			p_tree->_destroy( y->data );
		);

		y_is_red = y->is_red;
		p_tree->_free( y );
	}

	if( y_is_red == false ) /* y is black */
	{
		lc_rbtree_delete_fixup( p_tree, &x );
	}

	p_tree->size--;

	return true;
}

bool lc_rbtree_search( const lc_rbtree_t* p_tree, const void *key )
{
	lc_rbnode_t *x;
	assert( p_tree );

	x = p_tree->root;

	while( x != &RBNIL )
	{
		if( p_tree->_compare( key, x->data ) == 0 )
		{
			return true; /* found it */
		}
		else if( p_tree->_compare( key, x->data ) < 0 )
		{
			x = x->left;
		}
		else
		{
			x = x->right;
		}
	}

	return false;
}

void lc_rbtree_clear( lc_rbtree_t* p_tree )
{
	lc_rbnode_t *x;
	lc_rbnode_t *y;

	assert( p_tree );

	x = p_tree->root;
	//y = (lc_rbnode_t *) &RBNIL;

	while( p_tree->size > 0 )
	{
		y = x;

		/* find minimum... */
		while( y->left != &RBNIL )
		{
			y = y->left;
		}

		if( y->right == &RBNIL )
		{
			y->parent->left = (lc_rbnode_t *) &RBNIL;
			x = y->parent;

			/* free... */
			DESTROY_CHECK(
				p_tree->_destroy( y->data );
			);

			p_tree->_free( y );

			p_tree->size--;
		}
		else
		{
			x = y->right;
			x->parent = y->parent;

			if( x->parent == &RBNIL )
			{
				p_tree->root = x;
			}
			else
			{
				y->parent->left = x;
			}
			/* free... */
			DESTROY_CHECK(
				p_tree->_destroy( y->data );
			);

			p_tree->_free( y );

			p_tree->size--;
		}

		#ifdef _DEBUG_RBTREE
		y->data = NULL;
		#endif
	}

	/* reset the root and current pointers */
	p_tree->root = (lc_rbnode_t *) &RBNIL;
}

/* ------------------------------------- */

lc_rbnode_t *lc_rbnode_search( lc_rbtree_t* p_tree, const void *key )
{
	lc_rbnode_t *x;

	assert( p_tree );
	x = p_tree->root;

	while( x != &RBNIL )
	{
		if( p_tree->_compare( key, x->data ) == 0 )
		{
			return x;
		}
		else if( p_tree->_compare( key, x->data ) < 0 )
		{
			x = x->left;
		}
		else
		{
			x = x->right;
		}
	}

	return NULL;
}

lc_rbtree_iterator_t lc_rbtree_begin( const lc_rbtree_t* p_tree )
{
	assert( p_tree );
    return lc_rbnode_minimum( p_tree->root );
}

lc_rbtree_iterator_t lc_rbtree_end( )
{
	return (lc_rbnode_t *) &RBNIL;
}

bool lc_rbtree_serialize( lc_rbtree_t* p_tree, size_t element_size, FILE *file )
{
	bool result = true;
	size_t count;
	lc_rbtree_iterator_t itr;

	assert( p_tree );

	if( !p_tree )
	{
		result = false;
		goto done;
	}

	count = lc_rbtree_size(p_tree);
	if( fwrite( &count, sizeof(size_t), 1, file ) != 1 )
	{
		result = false;
		goto done;
	}


	for( itr = lc_rbtree_begin( p_tree );
	     itr != lc_rbtree_end( ) && !ferror(file);
	     itr = lc_rbtree_next( itr ) )
	{
		if( fwrite( itr->data, element_size, 1, file ) != 1 )
		{
			result = false;
			goto done;
		}
	}

done:
	return result;
}

bool lc_rbtree_unserialize( lc_rbtree_t* p_tree, size_t element_size, FILE *file )
{
	bool result = true;
	size_t count = 0;

	assert( p_tree );

	if( !p_tree )
	{
		result = false;
		goto done;
	}

	if( fread( &count, sizeof(size_t), 1, file ) != 1 )
	{
		result = false;
		goto done;
	}

	while( count > 0 && feof(file) == 0 )
	{
		void *p_data = p_tree->_alloc( element_size );

		if( fread( p_data, element_size, 1, file ) != 1 )
		{
			p_tree->_free( p_data );

			result = false;
			goto done;
		}

		lc_rbtree_insert( p_tree, p_data );
		count--;
	}

done:
	return result;
}


void lc_rbtree_alloc_set( lc_rbtree_t* p_tree, lc_alloc_fxn_t alloc )
{
	assert( p_tree );
	assert( alloc );
	p_tree->_alloc = alloc;
}

void lc_rbtree_free_set( lc_rbtree_t* p_tree, lc_free_fxn_t free )
{
	assert( p_tree );
	assert( free );
	p_tree->_free = free;
}


#ifdef _DEBUG_RBTREE
static bool lc_rbnode_verify_tree  ( lc_rbtree_t* p_tree, lc_rbnode_t *t );
static void padding             ( char ch, int n );
static void structure           ( const lc_rbnode_t *root, int level );

bool lc_rbtree_verify_tree( lc_rbtree_t* p_tree )
{
	assert( p_tree );

	/* check if root is black... */
	if( p_tree->root->is_red )
	{
		return false;
	}

	return lc_rbnode_verify_tree( p_tree, p_tree->root );
}

bool lc_rbnode_verify_tree( lc_rbtree_t* p_tree, lc_rbnode_t *t )
{
	assert( p_tree );

	if( t == &RBNIL )
	{
		return true;
	}

	if( t->is_red )
	{
		if( t->left->is_red || t->right->is_red )
		{
			return false;
		}
	}

	return lc_rbnode_verify_tree( p_tree, t->left ) &&
	       lc_rbnode_verify_tree( p_tree, t->right );
}

inline void padding( char ch, int n )
{
	int i;

	for( i = 0; i < n; i++ )
	{
		putchar( ch );
	}
}

inline void structure( const lc_rbnode_t *root, int level )
{
	int i;

	if( root == &RBNIL )
	{
		padding( '\t', level );
		puts( "~" );
	}
	else
	{
		structure( root->right, level + 1 );
		padding( '\t', level );
		printf( "%d-", *( (int *) root->data ) );
		printf( "%c\n", root->is_red ? 'R' : 'B' );
		structure( root->left, level + 1 );
	}
}

void lc_rbtree_print( const lc_rbtree_t* p_tree )
{
	structure( p_tree->root, 0 );
}
#endif

