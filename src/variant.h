#ifndef _VARIANT_H_
#define _VARIANT_H_
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

//#include "libcollections-config.h"
#include "types.h"
#include "tstring.h"
#ifdef __cplusplus
extern "C" {
#endif

struct variant;
typedef struct variant variant_t;


typedef enum _variant_type {
	VARIANT_NOT_INITIALIZED = 0,
	VARIANT_STRING,
	VARIANT_DECIMAL,
	VARIANT_INTEGER, 
	VARIANT_UNSIGNED_INTEGER, 
	VARIANT_POINTER,
	/* must be last one */
	VARIANT_TYPE_COUNT
} variant_type_t;

typedef union value {
   	tchar*          string;
	double          decimal;
	long            integer;
	unsigned long   unsigned_integer; 
	void*           pointer; /* must be last one */
} value_t;


#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
	/* untested */
	#define variant_get( p_variant )  _Generic( variant_type(p_variant), \
		VARIANT_STRING: variant_value(p_variant).string,  \
		VARIANT_DECIMAL: variant_value(p_variant).decimal,  \
		VARIANT_INTEGER: variant_value(p_variant).integer,  \
		VARIANT_UNSIGNED_INTEGER: variant_value(p_variant).unsigned_integer,  \
		VARIANT_POINTER: variant_value(p_variant).pointer,  \
		default: 0 \
		) (p_variant )
	#define variant_set( p_variant, value )  _Generic( (value), \
		tchar*: variant_set_string, \
		double:  variant_set_decimal, \
		long: variant_set_integer, \
		unsigned long: variant_set_unsigned_integer, \
		void*: variant_set_pointer,  \
		default:  variant_set_unsigned_integer\
		) (p_variant, value )
#endif


variant_t*     variant_create     ( variant_type_t type );
void           variant_destroy    ( variant_t* p_variant );
void           variant_initialize ( variant_t* p_variant, variant_type_t type, value_t value );
int            variant_compare    ( const variant_t* p_left, const variant_t* p_right ); 
boolean        variant_is_type    ( const variant_t* p_variant, variant_type_t type );
variant_type_t variant_type       ( const variant_t* p_variant );
void           variant_set_type   ( variant_t* p_variant, variant_type_t type );
value_t        variant_value      ( const variant_t* p_variant );
void           variant_set_value  ( variant_t* p_variant, value_t value );
void           variant_set_string( variant_t* p_variant, const tchar* value );
void           variant_set_decimal( variant_t* p_variant, double value );
void           variant_set_integer( variant_t* p_variant, long value );
void           variant_set_unsigned_integer( variant_t* p_variant, unsigned long value );
void           variant_set_pointer( variant_t* p_variant, const void* value );


#define    variant_create_string( )                  variant_create( VARIANT_STRING )
#define    variant_is_string( p_variant )            variant_is_type( p_variant, VARIANT_STRING )
#define    variant_string( p_variant )               (variant_value(p_variant).string)

#define    variant_create_decimal( )                 variant_create( VARIANT_DECIMAL )
#define    variant_is_decimal( p_variant )           variant_is_type( p_variant, VARIANT_DECIMAL )
#define    variant_decimal( p_variant )              (variant_value(p_variant).decimal)

#define    variant_create_integer( )                 variant_create( VARIANT_INTEGER )
#define    variant_is_integer( p_variant )           variant_is_type( p_variant, VARIANT_INTEGER )
#define    variant_integer( p_variant )              (variant_value(p_variant).integer)

#define    variant_create_unsigned_integer( )        variant_create( VARIANT_UNSIGNED_INTEGER )
#define    variant_is_unsigned_integer( p_variant )  variant_is_type( p_variant, VARIANT_UNSIGNED_INTEGER )
#define    variant_unsigned_integer( p_variant )     (variant_value(p_variant).unsigned_integer)

#define    variant_create_pointer( )                 variant_create( VARIANT_POINTER )
#define    variant_is_pointer( p_variant )           variant_is_type( p_variant, VARIANT_POINTER )
#define    variant_pointer( p_variant )              (variant_value(p_variant).pointer)

#ifdef __cplusplus
} /* external C linkage */
#endif
#endif /* _VARIANT_H_ */
