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
#include <string.h>
#include <assert.h>
#include "tstring.h"
#include "variant.h"


struct variant {
	variant_type_t type;
	value_t        value;
};


variant_t* variant_create( variant_type_t type )
{
	variant_t *p_variant = (variant_t *) malloc( sizeof(variant_t) );

	if( p_variant )
	{
		p_variant->type = type;
		memset( &p_variant->value, 0, sizeof(p_variant->value) );
	}

	return p_variant;
}

void variant_destroy( variant_t *p_variant )
{
	assert( p_variant );
	free( p_variant );
}

void variant_initialize( variant_t* p_variant, variant_type_t type, value_t value )
{
	assert( p_variant );
	p_variant->type  = type;
	p_variant->value = value;
}

int variant_compare( const variant_t* p_left, const variant_t* p_right )
{
	assert( p_left );
	assert( p_right );
	int result;

	if( p_left->type != p_right->type )
	{
		result = p_left->type - p_right->type;
	}
	else // types match
	{
		switch( p_left->type )
		{
			case VARIANT_STRING:
				result = tstrcmp( variant_string(p_left), variant_string(p_right) );
				break;
			case VARIANT_DECIMAL:
				result = variant_decimal(p_left) - variant_decimal(p_right);
				break;
			case VARIANT_INTEGER:
				result = variant_integer(p_left) - variant_integer(p_right);
				break;
			case VARIANT_UNSIGNED_INTEGER:
				result = variant_unsigned_integer(p_left) - variant_unsigned_integer(p_right);
				break;
			case VARIANT_POINTER:
				result = (unsigned char*) variant_pointer(p_left) - (unsigned char*) variant_pointer(p_right);
				break;
			default:
				assert( FALSE ); // Did someone add a new variant type?
				break;
		}
	}

	return result;
}

boolean variant_is_type( const variant_t *p_variant, variant_type_t type )
{
	assert( p_variant );
	return p_variant->type == type;
}

variant_type_t variant_type( const variant_t *p_variant )
{
	assert( p_variant );
	return p_variant->type;
}

void variant_set_type( variant_t *p_variant, variant_type_t type )
{
	assert( p_variant );
	p_variant->type = type;
}

value_t variant_value( const variant_t *p_variant )
{
	assert( p_variant );
	return p_variant->value;
}

void variant_set_value( variant_t *p_variant, value_t value )
{
	assert( p_variant );
	p_variant->value = value;
}

