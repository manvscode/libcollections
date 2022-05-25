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
#include <variant.h>

int main( int argc, char *argv[] )
{
	lc_variant_t* integer;
	lc_variant_t* decimal;
	lc_variant_t* string;
	lc_variant_t* boolean;
	lc_value_t    value;

	integer = lc_variant_create_integer( );
	decimal = lc_variant_create_decimal( );
	string  = lc_variant_create_string( );
	boolean = lc_variant_create_boolean( );

	value.integer = 42;
	lc_variant_set_value( integer, value );
	value.decimal = 3.14127;
	lc_variant_set_value( decimal, value );
	value.string = _T("This is a test string.");
	lc_variant_set_value( string, value );
	value.boolean = true;
	lc_variant_set_value( boolean, value );


	printf( "  Integer value is \"%ld\"\n", lc_variant_integer(integer) );
	printf( "  Decimal value is \"%lf\"\n", lc_variant_decimal(decimal) );
	#if defined(UNICODE)
	printf( "   String value is \"%ls\"\n", lc_variant_string(string) );
	#else
	printf( "   String value is \"%s\"\n", lc_variant_string(string) );
	#endif
	printf( "  Boolean value is \"%s\"\n", lc_variant_boolean(boolean) ? "TRUE" : "FALSE" );

	lc_variant_destroy( integer );
	lc_variant_destroy( decimal );
	lc_variant_destroy( string );
	lc_variant_destroy( boolean );

	return 0;
}
