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
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <bitset.h>

void print_bits( );

lc_bitset_t bits;

int main( int argc, char *argv[] )
{
	int i;
	bool r = true;

	r = lc_bitset_create( &bits, 8 );
	assert( r );


	printf( "bits = %ld\n\n", lc_bitset_bits( &bits ) );
	printf( "0123456789\n");


	lc_bitset_set( &bits, 0 );
	lc_bitset_set( &bits, 1 );
	lc_bitset_set( &bits, 2 );
	lc_bitset_set( &bits, 3 );
	lc_bitset_set( &bits, 4 );
	lc_bitset_set( &bits, 5 );
	lc_bitset_set( &bits, 6 );
	lc_bitset_set( &bits, 7 );

	print_bits( );



	if( lc_bitset_test( &bits, 7 ) )
	{
		printf( "7th bit is set\n" );
	}


	r = lc_bitset_resize( &bits, 64 );
	assert( r );

	lc_bitset_set( &bits, 9 );
	lc_bitset_unset( &bits, 8 );
	lc_bitset_set( &bits, 37 );
	lc_bitset_set( &bits, 38 );

	print_bits( );

	r = lc_bitset_resize( &bits, 128 );
	assert( r );


	for( i = 64; i < lc_bitset_bits(&bits); i++ )
	{
		lc_bitset_set( &bits, i );
	}

	print_bits( );

	lc_bitset_destroy( &bits );
	return 0;
}

void print_bits( )
{
	char *s = lc_bitset_string( &bits );

	printf( "%s\n", s );
	free( s );
}
