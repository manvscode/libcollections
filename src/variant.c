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
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "lc-string.h"
#include "variant.h"

lc_variant_t* lc_variant_create( lc_variant_type_t type )
{
	lc_variant_t *p_variant = (lc_variant_t *) malloc( sizeof(lc_variant_t) );

	if( p_variant )
	{
		p_variant->type = type;
		memset( &p_variant->value, 0, sizeof(p_variant->value) );
	}

	return p_variant;
}

void lc_variant_destroy( lc_variant_t *p_variant )
{
	assert( p_variant );
	free( p_variant );
}

void lc_variant_initialize( lc_variant_t* p_variant, lc_variant_type_t type, value_t value )
{
	assert( p_variant );
	p_variant->type  = type;
	p_variant->value = value;
}

int lc_variant_compare( const lc_variant_t* p_left, const lc_variant_t* p_right )
{
	int result;
	assert( p_left );
	assert( p_right );

	if( p_left->type != p_right->type )
	{
		result = p_left->type - p_right->type;
	}
	else /* types match */
	{
		switch( p_left->type )
		{
			case LC_VARIANT_STRING:
			{
				result = lc_strcmp( lc_variant_string(p_left), lc_variant_string(p_right) );
				break;
			}
			case LC_VARIANT_DECIMAL:
			{
				result = lc_variant_decimal(p_left) - lc_variant_decimal(p_right);
				break;
			}
			case LC_VARIANT_INTEGER:
			{
				long res = lc_variant_integer(p_left) - lc_variant_integer(p_right);
				result = res > 0 ? 1 : (res < 0 ? -1 : 0);
				break;
			}
			case LC_VARIANT_UNSIGNED_INTEGER:
			{
				long res = lc_variant_unsigned_integer(p_left) - lc_variant_unsigned_integer(p_right);
				result = res > 0 ? 1 : (res < 0 ? -1 : 0);
				break;
			}
			case LC_VARIANT_BOOLEAN:
			{
				result = lc_variant_boolean(p_left) == lc_variant_boolean(p_right);
				break;
			}
			case LC_VARIANT_POINTER:
			{
				long res = (unsigned char*) lc_variant_pointer(p_left) - (unsigned char*) lc_variant_pointer(p_right);
				result = res > 0 ? 1 : (res < 0 ? -1 : 0);
				break;
			}
			default:
			{
				assert( false ); /* Did someone add a new variant type? */
				result = 0;
				break;
			}
		}
	}

	return result;
}

bool lc_variant_is_type( const lc_variant_t *p_variant, lc_variant_type_t type )
{
	assert( p_variant );
	return p_variant->type == type;
}

lc_variant_type_t lc_variant_type( const lc_variant_t *p_variant )
{
	assert( p_variant );
	return p_variant->type;
}

void lc_variant_set_type( lc_variant_t *p_variant, lc_variant_type_t type )
{
	assert( p_variant );
	p_variant->type = type;
}

value_t lc_variant_value( const lc_variant_t *p_variant )
{
	assert( p_variant );
	return p_variant->value;
}

void lc_variant_set_value( lc_variant_t *p_variant, value_t value )
{
	assert( p_variant );
	p_variant->value = value;
}

void lc_variant_set_string( lc_variant_t* p_variant, const lc_char_t* value )
{
	assert( p_variant );
	p_variant->value.string = (lc_char_t*) value;
}

void lc_variant_set_decimal( lc_variant_t* p_variant, double value )
{
	assert( p_variant );
	p_variant->value.decimal = value;
}

void lc_variant_set_integer( lc_variant_t* p_variant, long value )
{
	assert( p_variant );
	p_variant->value.integer = value;
}

void lc_variant_set_unsigned_integer( lc_variant_t* p_variant, unsigned long value )
{
	assert( p_variant );
	p_variant->value.unsigned_integer = value;
}

void lc_variant_set_boolean( lc_variant_t* p_variant, bool value )
{
	assert( p_variant );
	p_variant->value.boolean = value;
}

void lc_variant_set_pointer( lc_variant_t* p_variant, const void* value )
{
	assert( p_variant );
	p_variant->value.pointer = (void*) value;
}

