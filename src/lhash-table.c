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
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "lhash-table.h"

static bool lc_lhash_table_find_bucket_for_insertion( lc_lhash_table_t* p_table, const void *data, size_t *p_index );
static bool lc_lhash_table_find_bucket( lc_lhash_table_t* p_table, const void *data, size_t *p_index );


#define bucket_is_occupied( p_table, bucket )    lc_bitset_test( &(p_table)->occupied, bucket )
#define bucket_mark_occupied( p_table, bucket )  lc_bitset_set( &(p_table)->occupied, bucket )
#define bucket_mark_empty( p_table, bucket )     lc_bitset_unset( &(p_table)->occupied, bucket )


#define bucket_is_deleted( p_table, bucket )     lc_bitset_test( &(p_table)->deleted, bucket )
#define bucket_mark_deleted( p_table, bucket )   lc_bitset_set( &(p_table)->deleted, bucket )
#define bucket_mark_available( p_table, bucket ) lc_bitset_unset( &(p_table)->deleted, bucket )

bool lc_lhash_table_create  ( lc_lhash_table_t* p_table, size_t element_size, size_t table_size,
                           lc_lhash_table_hash_fxn_t hash_function,
                           lc_lhash_table_compare_fxn_t compare_function,
                           lc_alloc_fxn_t alloc,
                           lc_free_fxn_t free )
{
	assert( p_table );

	if( lc_bitset_create( &p_table->occupied, table_size ) &&
	    lc_bitset_create( &p_table->deleted, table_size ) )
	{
		p_table->size             = 0;
		p_table->hash_callback    = hash_function;
		p_table->compare_callback = compare_function;

		return lc_array_create( &p_table->table, element_size, table_size, alloc, free );
	}

	return false;
}

void lc_lhash_table_destroy( lc_lhash_table_t* p_table )
{
	assert( p_table );
	lc_array_destroy( &p_table->table );
	lc_bitset_destroy( &p_table->occupied );
	lc_bitset_destroy( &p_table->deleted );
}

bool lc_lhash_table_find_bucket_for_insertion( lc_lhash_table_t* p_table, const void *data, size_t *p_index )
{
	bool result  = false;
	size_t hash     = p_table->hash_callback( data );
	size_t count;

	*p_index  = hash % lc_array_size( &p_table->table );

	for( count = 0; count < lc_array_size(&p_table->table) && !result; count++ )
	{
		if( bucket_is_deleted( p_table, *p_index ) || !bucket_is_occupied( p_table, *p_index ) )
		{
			bucket_mark_available( p_table, *p_index );
			result = true;
		}
		else
		{
			*p_index  = (*p_index + LC_LHASH_TABLE_LINEAR_CONSTANT) % lc_array_size( &p_table->table );
		}
	}

	return result;
}

bool lc_lhash_table_find_bucket( lc_lhash_table_t* p_table, const void *data, size_t *p_index )
{
	bool result        = false;
	size_t hash           = p_table->hash_callback( data );
	bool found_deleted = false;
	size_t deleted;
	size_t count;


	*p_index  = hash % lc_array_size( &p_table->table );
	deleted   = *p_index;

	for( count = 0; count < lc_array_size(&p_table->table) && !result; count++ )
	{
		if( bucket_is_deleted( p_table, *p_index ) )
		{
			/* Save the deleted index so that if we find an occupied matching element
 			 * we can move them into the deleted bucket and mark the original bucket
 			 * as
 			 */
			deleted = *p_index;
			*p_index  = (*p_index + LC_LHASH_TABLE_LINEAR_CONSTANT) % lc_array_size( &p_table->table );
			found_deleted = true;
			continue;
		}
		else
		if( bucket_is_occupied(p_table, *p_index) && p_table->compare_callback( lc_array_element( &p_table->table, *p_index ), data ) == 0 )
		{
			/* We found an item */

			if( found_deleted )
			{
				memmove( lc_array_element(&p_table->table, deleted), lc_array_element(&p_table->table, *p_index), lc_array_element_size(&p_table->table) );

				assert( !bucket_is_deleted( p_table, *p_index ) );
				bucket_mark_deleted( p_table, *p_index );
				bucket_mark_available( p_table, deleted );
				bucket_mark_occupied( p_table, deleted );

				*p_index = deleted;
			}

			result = true;
		}
		else if( !bucket_is_occupied( p_table, *p_index ) )
		{
			/* We didn't find an item but we did
 			 * find an empty bucket.
 			 */
			result = false;
		}
		else
		{
			*p_index  = (*p_index + LC_LHASH_TABLE_LINEAR_CONSTANT) % lc_array_size( &p_table->table );
		}
	}

	return result;
}

bool lc_lhash_table_insert( lc_lhash_table_t* p_table, const void *data )
{
	size_t index;
	bool result = false;

	assert( p_table );

	if( lc_lhash_table_find_bucket_for_insertion( p_table, data, &index ) )
	{
		memcpy( lc_array_element( &p_table->table, index ), data, lc_array_element_size(&p_table->table) );
		bucket_mark_occupied( p_table, index );

		p_table->size++;

		result = true;
	}

	return result;
}

bool lc_lhash_table_remove( lc_lhash_table_t* p_table, const void *data )
{
	size_t index;
	bool result = false;

	assert( p_table );

	if( lc_lhash_table_find_bucket( p_table, data, &index ) )
	{
		/* Mark the bucket as unoccupied and deleted */
		/*bucket_mark_empty( p_table, index ); // not sure if needed*/
		bucket_mark_deleted( p_table, index );
		p_table->size--;
		result = true;
	}

	return result;
}

bool lc_lhash_table_find( lc_lhash_table_t* p_table, const void *data, void **found_data )
{
	size_t index;

	*found_data = NULL;

	assert( p_table );

	if( lc_lhash_table_find_bucket( p_table, data, &index ) )
	{
		if( bucket_is_occupied( p_table, index ) )
		{
			*found_data = lc_array_element( &p_table->table, index );
			return true;
		}
	}

	return false;
}

void lc_lhash_table_clear( lc_lhash_table_t* p_table )
{
	lc_bitset_clear( &p_table->occupied );
	lc_bitset_clear( &p_table->deleted );
}

bool lc_lhash_table_resize( lc_lhash_table_t* p_table, size_t new_size )
{
	assert( p_table );

	if( new_size != lc_lhash_table_size(p_table) )
	{
		lc_lhash_table_t new_table;

		if( lc_lhash_table_create( &new_table, lc_array_element_size(&p_table->table), new_size, p_table->hash_callback, p_table->compare_callback, p_table->alloc, p_table->free ) )
		{
			size_t i;

			for( i = 0; i < lc_array_size(&p_table->table); i++ )
			{
				if( bucket_is_deleted( p_table, i ) )
				{
					continue;
				}
				else if( bucket_is_occupied( p_table, i ) )
				{
					lc_lhash_table_insert( &new_table, lc_array_element( &p_table->table, i ) );
				}
			}

			lc_lhash_table_destroy( p_table );
			*p_table = new_table;

			return true;
		}
	}

	return false;
}

bool lc_lhash_table_rehash( lc_lhash_table_t* p_table, double load_factor )
{
#if defined(LHASH_GROW_AND_SHRINK)
	double current_load;
	double upper_limit;
	double lower_limit;

	assert( load_factor > 0.0 );
	assert( load_factor < 1.0 );

	current_load = lc_lhash_table_load_factor( p_table );
	upper_limit  = load_factor * (1.0f + LC_LHASH_TABLE_THRESHOLD);
	lower_limit  = load_factor * (1.0f - LC_LHASH_TABLE_THRESHOLD);

	if( current_load > upper_limit )
	{
		size_t size = ((current_load) / upper_limit) * lc_lhash_table_size(p_table) + 1;
		/*  Load exceeds load factor threshold. We must increase the
 		 *  size to return the hash table to the desired load factor.
 		 */
		return lc_lhash_table_resize( p_table, size );
	}
	else if( current_load < lower_limit )
	{
		size_t size = (current_load / (lower_limit)) * lc_lhash_table_size(p_table) + 1;
		/*  Load exceeds load factor threshold. We must decrease the
 		 *  size to return the hash table to the desired load factor.
 		 */
		return lc_lhash_table_resize( p_table, size );
	}
#else
	double current_load;
	double upper_limit;

	assert( load_factor > 0.0 );
	assert( load_factor < 1.0 );

	current_load = lc_lhash_table_load_factor( p_table );
	upper_limit  = load_factor * (1.0f + LC_LHASH_TABLE_THRESHOLD);

	if( current_load > upper_limit )
	{
		size_t size = 2 * lc_lhash_table_size(p_table) + 1;
		/*  Load exceeds load factor threshold. We must increase the
 		 *  size to return the hash table to the desired load factor.
 		 */
		return lc_lhash_table_resize( p_table, size );
	}
#endif

	return false;
}


void lc_lhash_table_debug( lc_lhash_table_t* p_table )
{
	size_t count;

	printf( "[" );
	for( count = 0; count < lc_array_size(&p_table->table); count++ )
	{
		char bucket = '-';

		if( bucket_is_occupied( p_table, count ) ) bucket = 'O';
		if( bucket_is_deleted( p_table, count ) )  bucket = 'd';
		printf( "%c", bucket );
	}
	printf( "]" );
}
