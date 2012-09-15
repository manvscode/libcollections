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
#include <string.h>
#include <assert.h>
#include "hash-functions.h"
#include "types.h"

/*
 *   Hash Functions
 */
size_t pointer_hash( const void *data )
{
	return (size_t) data;
}

size_t string_hash( const void *string_addr )
{
	const char *str   = (const char*) string_addr;
	size_t hash_code  = 0;
	int length        = strlen( str );
	int i;

	for( i = 0; i < length; i++ )
	{
		hash_code = 31 * hash_code + str[ i ]; 
	}

	return hash_code;
}


size_t memory_hash( const void *p_memory, size_t size )
{
	const unsigned char *bytes = (const unsigned char*) p_memory;
	size_t hash_code           = 0;
	size_t i;

	for( i = 0; i < size; i++ )
	{
		hash_code = 31 * hash_code + bytes[ i ]; 
	}

	return hash_code;
}

size_t ip_address_hash( const void *data )
{
	unsigned short count;
	size_t hash;
	char *in_ip;
	char ip[ 24 ];
	char *token;
#ifdef THREAD_SAFE
	char* context = NULL;
#endif

	assert( data );
	
	count = 0;
	hash  = 0;

	in_ip = *((char **) data);
	strncpy( ip, in_ip, sizeof(ip) );
	ip[ sizeof(ip) - 1 ] = '\0';

#ifdef THREAD_SAFE
	token = strtok_s( (char *) ip, ".", &context );
#else
	token = strtok( (char *) ip, "." );
#endif

	while( token != NULL )
	{
		int part = atoi( token );

		hash |= (part << 8 * (3 - count));
	
#ifdef THREAD_SAFE
		token = strtok_s( NULL, ".", &context );
#else
		token = strtok( NULL, "." );
#endif
	}

	return hash;
}
