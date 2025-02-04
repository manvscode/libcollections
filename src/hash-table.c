/*
 * Copyright (C) 2010-2025 by Joseph A. Marrero.  https://joemarrero.com/
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
#include <string.h>
#include <assert.h>
#include <math.h>
#include "hash-table.h"

/*
 * Hash Table Functions
 */
bool lc_hash_table_create( lc_hash_table_t* p_table, size_t table_size, lc_hash_table_hash_fxn_t hash_function, lc_hash_table_element_fxn_t destroy, lc_hash_table_compare_fxn_t compare, lc_alloc_fxn_t alloc, lc_free_fxn_t free )
{
	assert( p_table );

	p_table->size       = 0;
	p_table->table_size = table_size;
	p_table->alloc      = alloc;
	p_table->free       = free;
	p_table->table      = (lc_slist_t *) p_table->alloc( lc_hash_table_table_size(p_table) * sizeof(lc_slist_t) );
	p_table->hash       = hash_function;
	p_table->compare    = compare;
	p_table->destroy    = destroy;

	assert( p_table->table );

	if( p_table->table )
	{
		#ifdef LC_HASH_TABLE_PREALLOC
		int i;
		for( i = 0; i < lc_hash_table_table_size(p_table); i++ )
		{
			lc_slist_create( &p_table->table[ i ], p_table->destroy, p_table->alloc, p_table->free );
		}
		#else
		memset( p_table->table, 0, lc_hash_table_table_size(p_table) * sizeof(lc_slist_t) );
		#endif
	}

	return p_table->table != NULL;
}

void lc_hash_table_destroy( lc_hash_table_t* p_table )
{
	size_t i;

	for( i = 0; i < lc_hash_table_table_size(p_table); i++ )
	{
		#ifdef LC_HASH_TABLE_PREALLOC
		lc_slist_destroy( &p_table->table[ i ] );
		#else
		lc_slist_t *p_list = &p_table->table[ i ];
		if( p_list )
		{
			lc_slist_destroy( p_list );
		}
		#endif
	}

	p_table->free( p_table->table );
}

bool lc_hash_table_insert( lc_hash_table_t* p_table, const void *data )
{
	size_t index     = p_table->hash( data ) % lc_hash_table_table_size(p_table);
	lc_slist_t *p_list  = &p_table->table[ index ];

	#ifndef LC_HASH_TABLE_PREALLOC
	if( !p_list->head ) /* uninitialized list */
	{
		lc_slist_create( p_list, p_table->destroy, p_table->alloc, p_table->free );
	}
	#endif

	if( lc_slist_push( p_list, data ) )
	{
		p_table->size++;
		return true;
	}

	return false;
}

bool lc_hash_table_remove( lc_hash_table_t* p_table, const void *data )
{
	size_t index         = p_table->hash( data ) % lc_hash_table_table_size(p_table);
	lc_slist_t *p_list      = &p_table->table[ index ];
	lc_slist_node_t *p_prev = NULL;
	lc_slist_node_t *p_node;

	assert( p_list );
	p_node = p_list->head;

	while( p_node != NULL )
	{
		if( p_table->compare( p_node->data, data ) == 0 )
		{
			/* Usually this returns true */
			if( lc_slist_remove_next( p_list, p_prev ) )
			{
				p_table->size--;
				return true;
			}
			else
			{
				return false;
			}
		}

		p_prev = p_node;
		p_node = p_node->next;
	}

	/* nothing found */
	return false;
}

bool lc_hash_table_find( const lc_hash_table_t* p_table, const void *data, void **found_data )
{
	size_t index    = p_table->hash( data ) % lc_hash_table_table_size(p_table);
	lc_slist_t *p_list = &p_table->table[ index ];
	lc_slist_node_t *p_node;

	assert( found_data );
	assert( p_list );

	p_node = p_list->head;

	while( p_node != NULL )
	{
		if( p_table->compare( p_node->data, data ) == 0 )
		{
			*found_data = p_node->data;
			assert( *found_data );
			return true;
		}

		p_node = p_node->next;
	}

	/* nothing found */
	*found_data = NULL;
	return false;
}

void lc_hash_table_clear( lc_hash_table_t* p_table )
{
	size_t i;

	for( i = 0; i < lc_hash_table_table_size(p_table); i++ )
	{
		#ifdef LC_HASH_TABLE_PREALLOC
		lc_slist_clear( &p_table->table[ i ] );
		#else
		lc_slist_t *p_list = &p_table->table[ i ];
		if( p_list )
		{
			lc_slist_clear( p_list );
		}
		#endif
	}

	p_table->size = 0;
}

bool lc_hash_table_resize( lc_hash_table_t* p_table, size_t new_size )
{
	if( new_size != lc_hash_table_size(p_table) )
	{
		lc_slist_t *p_new_table = (lc_slist_t *) p_table->alloc( new_size * sizeof(lc_slist_t) );
		lc_slist_t *p_old_table = p_table->table;
		size_t old_size    = p_table->table_size;
		size_t i;

		assert( new_size > 0 );

		if( !p_new_table )
		{
			return false;
		}

		p_table->size       = 0;
		p_table->table      = p_new_table;
		p_table->table_size = new_size;

		#ifdef LC_HASH_TABLE_PREALLOC
		for( i = 0; i < lc_hash_table_table_size(p_table); i++ )
		{
			lc_slist_create( &p_table->table[ i ], p_table->destroy, p_table->alloc, p_table->free );
		}
		#else
		memset( p_table->table, 0, new_size * sizeof(lc_slist_t) );
		#endif

		for( i = 0; i < old_size; i++ )
		{
			lc_slist_t *p_list = &p_old_table[ i ];
			p_list->destroy = NULL; /* prevent the data from being freed */

			if( p_list )
			{
				lc_slist_node_t *p_node = lc_slist_head( p_list );
				while( p_node )
				{
					lc_slist_node_t *p_previous;
					void *data = p_node->data;

					/* Re-insert data into new table */
					lc_hash_table_insert( p_table, data );

					p_previous = p_node;
					p_node     = p_node->next;

					p_table->free( p_previous );
				}
			}
		}

		p_table->free( p_old_table );

		return true;
	}

	return false;
}

bool lc_hash_table_rehash( lc_hash_table_t* p_table, double load_factor )
{
	double current_load = lc_hash_table_load_factor( p_table );

	double upper_limit = load_factor * (1.0f + LC_HASH_TABLE_THRESHOLD);
	double lower_limit = load_factor * (1.0f - LC_HASH_TABLE_THRESHOLD);

	if( current_load > upper_limit )
	{
		size_t size = (size_t)((current_load) / upper_limit) * lc_hash_table_size(p_table) + 1;
		/*  Load exceeds load factor threshold. We must increase the
 		 *  size to return the hash table to the desired load factor.
 		 */
		return lc_hash_table_resize( p_table, size );
	}
	else if( current_load < lower_limit )
	{
		size_t size = (size_t)(current_load / (lower_limit)) * lc_hash_table_size(p_table) + 1;
		/*  Load exceeds load factor threshold. We must decrease the
 		 *  size to return the hash table to the desired load factor.
 		 */
		return lc_hash_table_resize( p_table, size );
	}

	return false;
}

bool lc_hash_table_serialize( lc_hash_table_t* p_table, size_t element_size, FILE *file )
{
	bool result = true;
	size_t count;
	size_t i;

	if( !p_table )
	{
		result = false;
		goto done;
	}

	count = lc_hash_table_size(p_table);
	if( fwrite( &count, sizeof(size_t), 1, file ) != 1 )
	{
		result = false;
		goto done;
	}

	for( i = 0; i < lc_hash_table_table_size(p_table); i++ )
	{
		lc_slist_t *p_list = &p_table->table[ i ];
		if( p_list )
		{
			lc_slist_node_t *p_node = p_list->head;

			while( p_node != NULL )
			{
				if( fwrite( p_node->data, element_size, 1, file ) != 1 )
				{
					result = false;
					goto done;
				}

				p_node = p_node->next;
			}
		}
	}
done:
	return result;
}

bool lc_hash_table_unserialize( lc_hash_table_t* p_table, size_t element_size, FILE *file )
{
	bool result = true;
	size_t count = 0;

	if( !p_table )
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
		void *p_data = p_table->alloc( element_size );

		if( fread( p_data, element_size, 1, file ) != 1 )
		{
			p_table->free( p_data );
			result = false;
			goto done;
		}

		lc_hash_table_insert( p_table, p_data );
		count--;
	}

done:
	return result;
}

void lc_hash_table_iterator( const lc_hash_table_t* p_table, lc_hash_table_iterator_t* iter )
{
	assert( p_table );
	assert( iter );

	iter->table   = p_table;
	iter->index   = 0;
	iter->current = NULL;
	iter->data    = NULL;
}

bool lc_hash_table_iterator_next( lc_hash_table_iterator_t* iter )
{
	bool result;

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
		while( iter->index < lc_hash_table_table_size(iter->table) )
		{
			lc_slist_t *p_list = &iter->table->table[ iter->index ];

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
		result = true;
	}
	else
	{
		iter->data = NULL;
		result = false;
	}

	return result;
}

void* lc_hash_table_iterator_data( lc_hash_table_iterator_t* iter )
{
	assert( iter );
	return iter->data;
}
