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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "hash-map.h"

/*
 *  List Functions
 */
struct hash_map_node {
	void *key;
	void *value;
	struct hash_map_node* next;	
};

struct hash_map_list {
	hash_map_node_t* head;
	//size_t size;
	hash_map_element_function destroy;
};

static void    hm_list_create        ( hash_map_list_t *p_list, hash_map_element_function destroy );
static void    hm_list_destroy       ( hash_map_t *p_map, hash_map_list_t *p_list );
static boolean hm_list_insert_front  ( hash_map_t *p_map, hash_map_list_t *p_list, const void *key, const void *value ); /* O(1) */
static boolean hm_list_remove_front  ( hash_map_t *p_map, hash_map_list_t *p_list ); /* O(1) */
static boolean hm_list_remove_next   ( hash_map_t *p_map, hash_map_list_t *p_list, hash_map_node_t *p_front_node ); /* O(1) */ 
static void    hm_list_clear         ( hash_map_t *p_map, hash_map_list_t *p_list ); /* O(N) */
#define hm_list_head(p_list)       ((p_list)->head)
#define hm_list_size(p_list)       ((p_list)->size)
#define hm_list_is_empty(p_list)   ((p_list)->size <= 0)

#if defined(HASH_MAP_DESTROY_CHECK) || defined(DESTROY_CHECK_ALL)
	#define DESTROY_CHECK( code ) \
		if( p_list->destroy ) \
		{ \
			code \
		}
#else
	#define DESTROY_CHECK( code ) \
		code 
#endif

/*
 * Hash Map Functions
 */
#ifdef USE_ALLOCATORS
boolean hash_map_create( hash_map_t *p_map, size_t table_size, hash_map_hash_function hash_function, hash_map_element_function destroy, hash_map_compare_function compare, alloc_function alloc, free_function free )
#else
boolean hash_map_create( hash_map_t *p_map, size_t table_size, hash_map_hash_function hash_function, hash_map_element_function destroy, hash_map_compare_function compare )
#endif
{
	assert( p_map );
	#ifdef HASH_MAP_PREALLOC
	int i;
	#endif

	#ifdef USE_ALLOCATORS
	p_map->alloc = alloc;
	p_map->free  = free;
	#endif

	p_map->size       = 0;
	p_map->table_size = table_size;
	#ifdef USE_ALLOCATORS
	p_map->table      = (hash_map_list_t *) p_map->alloc( hash_map_table_size(p_map) * sizeof(hash_map_list_t) );
	#else
	p_map->table      = (hash_map_list_t *) malloc( hash_map_table_size(p_map) * sizeof(hash_map_list_t) );
	#endif
	p_map->hash       = hash_function;
	p_map->compare    = compare;
	p_map->destroy    = destroy;

	assert( p_map->table );

	if( p_map->table )
	{
		#ifdef HASH_MAP_PREALLOC
		for( i = 0; i < hash_map_table_size(p_map); i++ )
		{
			hm_list_create( &p_map->table[ i ], p_map->destroy );
		}
		#else
		memset( p_map->table, 0, hash_map_table_size(p_map) * sizeof(hash_map_list_t) );
		#endif
	}

	return p_map->table != NULL;
}

void hash_map_destroy( hash_map_t *p_map )
{
	size_t i;
	assert( p_map );

	for( i = 0; i < hash_map_table_size(p_map); i++ )
	{
		#ifdef HASH_MAP_PREALLOC
		hm_list_destroy( p_map, &p_map->table[ i ] );
		#else
		hash_map_list_t *p_list = &p_map->table[ i ];
		if( p_list )
		{
			hm_list_destroy( p_map, p_list );
		}
		#endif
	}

	#ifdef USE_ALLOCATORS
	p_map->free( p_map->table );
	#else
	free( p_map->table );
	#endif
}

boolean hash_map_insert( hash_map_t *p_map, const void *key, const void *value )
{
	size_t index;
	hash_map_list_t *p_list;

	assert( p_map );
	
	index   = p_map->hash( key ) % hash_map_table_size(p_map);
	p_list  = &p_map->table[ index ];

	#ifndef HASH_MAP_PREALLOC
	if( !p_list->head )
	{
		hm_list_create( p_list, p_map->destroy );
	}
	#endif

	
	if( hm_list_insert_front( p_map, p_list, key, value ) )
	{
		p_map->size++;
		return TRUE;
	}
	
	return FALSE;
}

boolean hash_map_remove( hash_map_t *p_map, const void *key )
{
	size_t index;
	hash_map_node_t *p_prev;
	hash_map_list_t *p_list;
	hash_map_node_t *p_node;
	
	assert( p_map );

	index  = p_map->hash( key ) % hash_map_table_size(p_map);
	p_prev = NULL;
	p_list = &p_map->table[ index ];

	assert( p_list );
	p_node = p_list->head;	

	while( p_node != NULL )
	{
		if( strcmp(p_node->key, key) == 0 )
		{
			/* Usually this returns TRUE */
			if( hm_list_remove_next( p_map, p_list, p_prev ) )
			{
				p_map->size--;
				return TRUE;	
			}
			else
			{
				assert( FALSE );
				return FALSE;
			}
		}

		p_prev = p_node;
		p_node = p_node->next;
	}	

	/* nothing found */
	return FALSE;
}

boolean hash_map_find( const hash_map_t *p_map, const void *key, void **value )
{
	size_t index;
	hash_map_list_t *p_list;
	hash_map_node_t *p_node;

	
	assert( p_map );

	index  = p_map->hash( key ) % hash_map_table_size(p_map);
	p_list = &p_map->table[ index ];

	assert( p_list );
	p_node = p_list->head;	

	while( p_node != NULL )
	{
		if( strcmp(p_node->key, key) == 0 )
		{
			*value = p_node->value;	
			return TRUE;
		}

		p_node = p_node->next;
	}	

	/* nothing found */
	*value = NULL;
	return FALSE;
}

void hash_map_clear( hash_map_t *p_map )
{
	size_t i;
	assert( p_map );

	for( i = 0; i < hash_map_table_size(p_map); i++ )
	{
		#ifdef HASH_MAP_PREALLOC
		hm_list_clear( p_map, &p_map->table[ i ] );
		#else
		hash_map_list_t *p_list = &p_map->table[ i ];
		if( p_list )
		{
			hm_list_clear( p_map, p_list );
		}
		#endif
		
	}

	p_map->size = 0;
}


/*
 * List Functions
 */
void hm_list_create( hash_map_list_t *p_list, hash_map_element_function destroy )
{
	assert( p_list );
	p_list->head    = NULL;
	p_list->destroy = destroy;
}

void hm_list_destroy( hash_map_t *p_map, hash_map_list_t *p_list )
{
	hm_list_clear( p_map, p_list );

	#ifdef _HASH_MAP_DEBUG
	p_list->head = NULL;
	#endif
}

boolean hm_list_insert_front( hash_map_t *p_map, hash_map_list_t *p_list, const void *key, const void *value ) /* O(1) */ 
{
	hash_map_node_t *p_node;
	assert( p_list );

	#ifdef USE_ALLOCATORS
	p_node = (hash_map_node_t *) p_map->alloc( sizeof(hash_map_node_t) );
	#else
	p_node = (hash_map_node_t *) malloc( sizeof(hash_map_node_t) );
	#endif
	assert( p_node );

	if( p_node != NULL )
	{
		p_node->key   = (void *) key;
		p_node->value = (void *) value;
		p_node->next  = p_list->head;

		p_list->head = p_node;

		return TRUE;
	}

	return FALSE;
}

boolean hm_list_remove_front( hash_map_t *p_map, hash_map_list_t *p_list ) /* O(1) */
{
	boolean result = TRUE;
	hash_map_node_t *p_node;

	assert( p_list );

	p_node = p_list->head->next;

	DESTROY_CHECK( 
		result = p_list->destroy( p_list->head->key, p_list->head->value );
	);

	#ifdef USE_ALLOCATORS
	p_map->free( p_list->head );
	#else
	free( p_list->head );
	#endif

	p_list->head = p_node;

	return result;
}

boolean hm_list_remove_next( hash_map_t *p_map, hash_map_list_t *p_list, hash_map_node_t *p_front_node ) /* O(1) */ 
{
	assert( p_list );

	if( p_front_node )
	{
		boolean result            = TRUE;
		hash_map_node_t *p_node     = p_front_node->next;
		hash_map_node_t *p_new_next = p_node->next;

		DESTROY_CHECK( 
			result = p_list->destroy( p_node->key, p_node->value );
		);

		#ifdef USE_ALLOCATORS
		p_map->free( p_node );
		#else
		free( p_node );
		#endif

		p_front_node->next = p_new_next;

		return result;
	}

	return hm_list_remove_front( p_map, p_list );
}

void hm_list_clear( hash_map_t *p_map, hash_map_list_t *p_list )
{
	while( hm_list_head(p_list) )
	{
		hm_list_remove_front( p_map, p_list );
	}
}

boolean hash_map_resize( hash_map_t *p_map, size_t new_size )
{
	if( new_size != hash_map_size(p_map) )
	{
		#ifdef USE_ALLOCATORS
		hash_map_list_t *p_new_table = (hash_map_list_t *) p_map->alloc( new_size * sizeof(hash_map_list_t) );
		#else
		hash_map_list_t *p_new_table = (hash_map_list_t *) malloc( new_size * sizeof(hash_map_list_t) );
		#endif
		hash_map_list_t *p_old_table = p_map->table;
		size_t old_size              = p_map->table_size;	
		size_t i;

		assert( new_size > 0 );

		if( !p_new_table )
		{
			return FALSE;
		}
		
		p_map->size       = 0;
		p_map->table      = p_new_table;
		p_map->table_size = new_size;

		#ifdef HASH_MAP_PREALLOC
		for( i = 0; i < hash_map_table_size(p_map); i++ ) 
		{
			hm_list_create( &p_map->table[ i ], p_map->destroy );
		}
		#else
		memset( p_map->table, 0, new_size * sizeof(hash_map_list_t) );
		#endif

		for( i = 0; i < old_size; i++ ) 
		{
			hash_map_list_t *p_list = &p_old_table[ i ];

			if( p_list )
			{
				hash_map_node_t *p_node = hm_list_head( p_list );
				while( p_node )
				{
					hash_map_node_t *p_previous;
					void *key   = p_node->key;	
					void *value = p_node->value;	

					/* Re-insert data into new table */
					hash_map_insert( p_map, key, value );

					p_previous = p_node;
					p_node     = p_node->next;

					#ifdef USE_ALLOCATORS
					p_map->free( p_previous );
					#else
					free( p_previous );
					#endif
				}
			}
		}

		#ifdef USE_ALLOCATORS
		p_map->free( p_old_table );
		#else
		free( p_old_table );
		#endif

		return TRUE;
	}

	return FALSE;
}

boolean hash_map_rehash( hash_map_t *p_map, float load_factor )
{
	double current_load = hash_map_load_factor( p_map );

	double upper_limit = load_factor * (1.0f + HASH_MAP_THRESHOLD);
	double lower_limit = load_factor * (1.0f - HASH_MAP_THRESHOLD);

	if( current_load > upper_limit )
	{
		size_t size = (size_t)((current_load) / upper_limit) * hash_map_size(p_map) + 1;
		/*  Load exceeds load factor threshold. We must increase the
 		 *  size to return the hash map to the desired load factor.
 		 */
		return hash_map_resize( p_map, size ); 
	}
	else if( current_load < lower_limit )
	{
		size_t size = (size_t)(current_load / (lower_limit)) * hash_map_size(p_map) + 1;
		/*  Load exceeds load factor threshold. We must decrease the
 		 *  size to return the hash map to the desired load factor.
 		 */
		return hash_map_resize( p_map, size ); 
	}


	return FALSE;
}

boolean hash_map_serialize( hash_map_t *p_map, size_t key_size, size_t value_size, FILE *file )
{
	boolean result = TRUE;
	size_t count;
	size_t i;

	if( !p_map )
	{
		result = FALSE;
		goto done;
	}

	count = hash_map_size(p_map);

	if( fwrite( &count, sizeof(size_t), 1, file ) != 1 )
	{
		result = FALSE;
		goto done;
	}

	for( i = 0; i < hash_map_table_size(p_map); i++ )
	{
		hash_map_list_t *p_list = &p_map->table[ i ];
		if( p_list )
		{
			hash_map_node_t *p_node = p_list->head;	

			while( p_node != NULL )
			{
				if( fwrite( p_node->key, key_size, 1, file ) != 1 )
				{
					result = FALSE;
					goto done;
				}

				if( fwrite( p_node->value, value_size, 1, file ) != 1 )
				{
					result = FALSE;
					goto done;
				}

				p_node = p_node->next;
			}	
		}
	}
done:	
	return result;
}

boolean hash_map_unserialize( hash_map_t *p_map, size_t key_size, size_t value_size, FILE *file )
{
	boolean result = TRUE;
	size_t count = 0;

	if( !p_map )
	{
		result = FALSE;
		goto done;
	}

	if( fread( &count, sizeof(size_t), 1, file ) != 1 )
	{
		result = FALSE;
		goto done;
	}

	while( count > 0 && feof(file) == 0 )
	{
		void *p_key;
		void *p_val;

		#ifdef USE_ALLOCATORS
		p_key = p_map->alloc( key_size );
		#else
		p_key = malloc( key_size );
		#endif

		if( fread( p_key, key_size, 1, file ) != 1 )
		{
			#ifdef USE_ALLOCATORS
			p_map->free( p_key );
			#else
			free( p_key );
			#endif
			result = FALSE;
			goto done;
		}

		#ifdef USE_ALLOCATORS
		p_val = p_map->alloc( value_size );
		#else
		p_val = malloc( value_size );
		#endif

		if( fread( p_val, value_size, 1, file ) != 1 )
		{
			#ifdef USE_ALLOCATORS
			p_map->free( p_key );
			p_map->free( p_val );
			#else
			free( p_key );
			free( p_val );
			#endif
			result = FALSE;
			goto done;
		}

		hash_map_insert( p_map, p_key, p_val );
		count--;
	}

done:	
	return result;
}

#ifdef USE_ALLOCATORS
void hash_map_alloc_set( hash_map_t *p_map, alloc_function alloc )
{
	assert( p_map );
	assert( alloc );
	p_map->alloc = alloc;
}

void hash_map_free_set( hash_map_t *p_map, free_function free )
{
	assert( p_map );
	assert( free );
	p_map->free = free;
}
#endif

void hash_map_iterator( const hash_map_t* p_map, hash_map_iterator_t* iter )
{
	assert( p_map );
	assert( iter );

	iter->map     = p_map;
	iter->index   = 0;
	iter->current = NULL;
	iter->key     = NULL;
	iter->value   = NULL;
}

boolean hash_map_iterator_next( hash_map_iterator_t* iter )
{
	boolean result;

	assert( iter );

	if( iter->current )
	{
		if( iter->current->next )
		{
			iter->current = iter->current->next;
		}
		else
		{
			iter->index++;
			iter->current = NULL;
		}
	}

	if( !iter->current )
	{
		while( iter->index < hash_map_table_size(iter->map) )
		{
			hash_map_list_t *p_list = &iter->map->table[ iter->index ];

			if( p_list && p_list->head )
			{
				iter->current = p_list->head;
				break;
			}
			
			iter->index++;
		}
	}

	if( iter->current )
	{
		iter->key   = iter->current->key;
		iter->value = iter->current->value;

		result = TRUE;
	}
	else
	{
		iter->key   = NULL;
		iter->value = NULL;
		
		result = FALSE;
	}

	return result;
}

void* hash_map_iterator_key( hash_map_iterator_t* iter )
{
	assert( iter );
	return iter->key;
}

void* hash_map_iterator_value( hash_map_iterator_t* iter )
{
	assert( iter );
	return iter->value;
}


