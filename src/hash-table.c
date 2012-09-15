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
#include "hash-table.h"

/*
 * Hash Table Functions
 */
#ifdef USE_ALLOCATORS
boolean hash_table_create( hash_table_t *p_table, size_t table_size, hash_table_hash_function hash_function, hash_table_element_function destroy, hash_table_compare_function compare, alloc_function alloc, free_function free )
#else
boolean hash_table_create( hash_table_t *p_table, size_t table_size, hash_table_hash_function hash_function, hash_table_element_function destroy, hash_table_compare_function compare ) 
#endif
{
	assert( p_table );

	p_table->size       = 0;
	p_table->table_size = table_size;
	#ifdef USE_ALLOCATORS
	p_table->alloc      = alloc;
	p_table->free       = free;
	p_table->table      = (slist_t *) p_table->alloc( hash_table_table_size(p_table) * sizeof(slist_t) );
	#else
	p_table->table      = (slist_t *) malloc( hash_table_table_size(p_table) * sizeof(slist_t) );
	#endif
	p_table->hash       = hash_function;
	p_table->compare    = compare;
	p_table->destroy    = destroy;

	assert( p_table->table );

	if( p_table->table )
	{
		#ifdef HASH_TABLE_PREALLOC
		int i;
		for( i = 0; i < hash_table_table_size(p_table); i++ ) 
		{
			#ifdef USE_ALLOCATORS
			slist_create( &p_table->table[ i ], p_table->destroy, p_table->alloc, p_table->free );
			#else
			slist_create( &p_table->table[ i ], p_table->destroy );
			#endif
		}
		#else
		memset( p_table->table, 0, hash_table_table_size(p_table) * sizeof(slist_t) );
		#endif
	}

	return p_table->table != NULL;
}

void hash_table_destroy( hash_table_t *p_table ) 
{
	size_t i;

	for( i = 0; i < hash_table_table_size(p_table); i++ ) 
	{
		#ifdef HASH_TABLE_PREALLOC
		slist_destroy( &p_table->table[ i ] );
		#else
		slist_t *p_list = &p_table->table[ i ];
		if( p_list )
		{
			slist_destroy( p_list );
		}
		#endif
	}

	#ifdef USE_ALLOCATORS
	p_table->free( p_table->table );
	#else
	free( p_table->table );
	#endif
}

boolean hash_table_insert( hash_table_t *p_table, const void *data ) 
{
	size_t index     = p_table->hash( data ) % hash_table_table_size(p_table);
	slist_t *p_list  = &p_table->table[ index ];

	#ifndef HASH_TABLE_PREALLOC
	if( !p_list->head ) /* uninitialized list */
	{
		#ifdef USE_ALLOCATORS
		slist_create( p_list, p_table->destroy, p_table->alloc, p_table->free );
		#else
		slist_create( p_list, p_table->destroy );
		#endif
	}
	#endif

	if( slist_push( p_list, data ) )
	{
		p_table->size++;
		return TRUE;
	}
	
	return FALSE;
}

boolean hash_table_remove( hash_table_t *p_table, const void *data ) 
{
	size_t index         = p_table->hash( data ) % hash_table_table_size(p_table);
	slist_t *p_list      = &p_table->table[ index ];
	slist_node_t *p_prev = NULL;
	slist_node_t *p_node;

	assert( p_list );
	p_node = p_list->head;	

	while( p_node != NULL ) 
	{
		if( p_table->compare( p_node->data, data ) == 0 ) 
		{
			/* Usually this returns TRUE */
			if( slist_remove_next( p_list, p_prev ) )
			{
				p_table->size--;
				return TRUE;	
			}
			else
			{
				return FALSE;
			}
		}

		p_prev = p_node;
		p_node = p_node->next;
	}	

	/* nothing found */
	return FALSE;
}

boolean hash_table_find( const hash_table_t *p_table, const void *data, void **found_data ) 
{
	size_t index    = p_table->hash( data ) % hash_table_table_size(p_table);
	slist_t *p_list = &p_table->table[ index ];
	slist_node_t *p_node;

	assert( found_data );
	assert( p_list );
	
	p_node = p_list->head;	

	while( p_node != NULL ) 
	{
		if( p_table->compare( p_node->data, data ) == 0 ) 
		{
			*found_data = p_node->data;	
			assert( *found_data );
			return TRUE;
		}

		p_node = p_node->next;
	}	

	/* nothing found */
	*found_data = NULL;
	return FALSE;
}

void hash_table_clear( hash_table_t *p_table ) 
{
	size_t i;

	for( i = 0; i < hash_table_table_size(p_table); i++ ) 
	{
		#ifdef HASH_TABLE_PREALLOC
		slist_clear( &p_table->table[ i ] );
		#else
		slist_t *p_list = &p_table->table[ i ];
		if( p_list )
		{
			slist_clear( p_list );
		}
		#endif
	}

	p_table->size = 0;
}

boolean hash_table_resize( hash_table_t *p_table, size_t new_size )
{
	if( new_size != hash_table_size(p_table) )
	{
		#ifdef USE_ALLOCATORS
		slist_t *p_new_table = (slist_t *) p_table->alloc( new_size * sizeof(slist_t) );
		#else
		slist_t *p_new_table = (slist_t *) malloc( new_size * sizeof(slist_t) );
		#endif
		slist_t *p_old_table = p_table->table;
		size_t old_size    = p_table->table_size;	
		size_t i;
		
		assert( new_size > 0 );

		if( !p_new_table )
		{
			return FALSE;
		}

		p_table->size       = 0;	
		p_table->table      = p_new_table;
		p_table->table_size = new_size;

		#ifdef HASH_TABLE_PREALLOC
		for( i = 0; i < hash_table_table_size(p_table); i++ ) 
		{
			#ifdef USE_ALLOCATORS
			slist_create( &p_table->table[ i ], p_table->destroy, p_table->alloc, p_table->free );
			#else
			slist_create( &p_table->table[ i ], p_table->destroy );
			#endif
		}
		#else
		memset( p_table->table, 0, new_size * sizeof(slist_t) );
		#endif

		for( i = 0; i < old_size; i++ ) 
		{
			slist_t *p_list = &p_old_table[ i ];
			p_list->destroy = NULL; /* prevent the data from being freed */

			if( p_list )
			{
				slist_node_t *p_node = slist_head( p_list );
				while( p_node )
				{
					slist_node_t *p_previous;
					void *data = p_node->data;	

					/* Re-insert data into new table */
					hash_table_insert( p_table, data );

					p_previous = p_node;
					p_node     = p_node->next;

					#ifdef USE_ALLOCATORS
					p_table->free( p_previous );
					#else
					free( p_previous );
					#endif
				}
			}
		}

		#ifdef USE_ALLOCATORS
		p_table->free( p_old_table );
		#else
		free( p_old_table );
		#endif

		return TRUE;
	}

	return FALSE;
}

boolean hash_table_rehash( hash_table_t *p_table, double load_factor )
{
	double current_load = hash_table_load_factor( p_table );

	double upper_limit = load_factor * (1.0f + HASH_TABLE_THRESHOLD);
	double lower_limit = load_factor * (1.0f - HASH_TABLE_THRESHOLD);

	if( current_load > upper_limit )
	{
		size_t size = (size_t)((current_load) / upper_limit) * hash_table_size(p_table) + 1;
		/*  Load exceeds load factor threshold. We must increase the
 		 *  size to return the hash table to the desired load factor.
 		 */
		return hash_table_resize( p_table, size ); 
	}
	else if( current_load < lower_limit )
	{
		size_t size = (size_t)(current_load / (lower_limit)) * hash_table_size(p_table) + 1;
		/*  Load exceeds load factor threshold. We must decrease the
 		 *  size to return the hash table to the desired load factor.
 		 */
		return hash_table_resize( p_table, size ); 
	}

	return FALSE;
}

boolean hash_table_serialize( hash_table_t *p_table, size_t element_size, FILE *file )
{
	boolean result = TRUE;
	size_t count;
	size_t i;

	if( !p_table )
	{
		result = FALSE;
		goto done;
	}

	count = hash_table_size(p_table);
	if( fwrite( &count, sizeof(size_t), 1, file ) != 1 )
	{
		result = FALSE;
		goto done;
	}

	for( i = 0; i < hash_table_table_size(p_table); i++ )
	{
		slist_t *p_list = &p_table->table[ i ];
		if( p_list )
		{
			slist_node_t *p_node = p_list->head;	

			while( p_node != NULL )
			{
				if( fwrite( p_node->data, element_size, 1, file ) != 1 )
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

boolean hash_table_unserialize( hash_table_t *p_table, size_t element_size, FILE *file )
{
	boolean result = TRUE;
	size_t count = 0;

	if( !p_table )
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
		#ifdef USE_ALLOCATORS
		void *p_data = p_table->alloc( element_size );
		#else
		void *p_data = malloc( element_size );
		#endif

		if( fread( p_data, element_size, 1, file ) != 1 )
		{
			#ifdef USE_ALLOCATORS
			p_table->free( p_data );
			#else
			free( p_data );
			#endif
			result = FALSE;
			goto done;
		}

		hash_table_insert( p_table, p_data );
		count--;
	}

done:	
	return result;
}

void hash_table_iterator( const hash_table_t* p_table, hash_table_iterator_t* iter )
{
	assert( p_table );
	assert( iter );

	iter->table   = p_table;
	iter->index   = 0;
	iter->current = NULL;
	iter->data    = NULL;
}

boolean hash_table_iterator_next( hash_table_iterator_t* iter )
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
		while( iter->index < hash_table_table_size(iter->table) )
		{
			slist_t *p_list = &iter->table->table[ iter->index ];

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
		iter->data = iter->current->data;
		result = TRUE;
	}
	else
	{
		iter->data = NULL;
		result = FALSE;
	}

	return result;
}

void* hash_table_iterator_data( hash_table_iterator_t* iter )
{
	assert( iter );
	return iter->data;
}
