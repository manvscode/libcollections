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
#ifndef _LC_VARIANT_H_
#define _LC_VARIANT_H_
/**
 * @file variant.h
 * @brief A variant type.
 *
 * A variant is an object that can hold one of several things.
 * It's useful for storing various datatypes in a collection.
 *
 * @defgroup lc_variant Variant
 * @ingroup Other
 *
 * @{
 */
#include <stdbool.h>
#include "lc-string.h"
#ifdef __cplusplus
extern "C" {
#endif


/**
 * The various data types that can be stored in
 * a lc_variant_t.
 */
typedef enum lc_variant_type {
	LC_VARIANT_NOT_INITIALIZED = 0,  /**< This variant is not initialized. */
	LC_VARIANT_STRING,               /**< This variant holds a string. */
	LC_VARIANT_DECIMAL,              /**< This variant holds a decimal. */
	LC_VARIANT_INTEGER,              /**< This variant holds an integer. */
	LC_VARIANT_UNSIGNED_INTEGER,     /**< This variant holds an unsigned integer. */
	LC_VARIANT_BOOLEAN,              /**< This variant holds a boolean. */
	LC_VARIANT_POINTER,              /**< This variant holds a pointer. */
	/* must be last one */
	LC_VARIANT_TYPE_COUNT
} lc_variant_type_t;

/**
 * This is the value type for the lc_variant_t.  It can
 * be a string, decimal, integer, unsigned integer, boolean,
 * or pointer.
 */
typedef union value {
   	lc_char_t*      string;
	double          decimal;
	long            integer;
	unsigned long   unsigned_integer;
	bool            boolean;
	void*           pointer; /* must be last one */
} value_t;

/**
 * This is a variant object.
 */
typedef struct lc_variant {
	lc_variant_type_t type;
	value_t        value;
} lc_variant_t;



#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
	/* untested */
	#define lc_variant_get( p_variant )  _Generic( lc_variant_type(p_variant), \
		LC_VARIANT_STRING: lc_variant_value(p_variant).string,  \
		LC_VARIANT_DECIMAL: lc_variant_value(p_variant).decimal,  \
		LC_VARIANT_INTEGER: lc_variant_value(p_variant).integer,  \
		LC_VARIANT_UNSIGNED_INTEGER: lc_variant_value(p_variant).unsigned_integer,  \
		LC_VARIANT_BOOLEAN: lc_variant_value(p_variant).boolean,  \
		LC_VARIANT_POINTER: lc_variant_value(p_variant).pointer,  \
		default: 0 \
		) (p_variant )
	#define lc_variant_set( p_variant, value )  _Generic( (value), \
		lc_char_t*: lc_variant_set_string, \
		double:  lc_variant_set_decimal, \
		long: lc_variant_set_integer, \
		unsigned long: lc_variant_set_unsigned_integer, \
		bool: lc_variant_set_boolean, \
		void*: lc_variant_set_pointer,  \
		default:  lc_variant_set_unsigned_integer\
		) (p_variant, value )
#endif


lc_variant_t*        lc_variant_create               ( lc_variant_type_t type );
void                 lc_variant_destroy              ( lc_variant_t* p_variant );
void                 lc_variant_initialize           ( lc_variant_t* p_variant, lc_variant_type_t type, value_t value );
int                  lc_variant_compare              ( const lc_variant_t* p_left, const lc_variant_t* p_right );
bool                 lc_variant_is_type              ( const lc_variant_t* p_variant, lc_variant_type_t type );
lc_variant_type_t    lc_variant_type                 ( const lc_variant_t* p_variant );
void                 lc_variant_set_type             ( lc_variant_t* p_variant, lc_variant_type_t type );
value_t              lc_variant_value                ( const lc_variant_t* p_variant );
void                 lc_variant_set_value            ( lc_variant_t* p_variant, value_t value );
void                 lc_variant_set_string           ( lc_variant_t* p_variant, const lc_char_t* value );
void                 lc_variant_set_decimal          ( lc_variant_t* p_variant, double value );
void                 lc_variant_set_integer          ( lc_variant_t* p_variant, long value );
void                 lc_variant_set_unsigned_integer ( lc_variant_t* p_variant, unsigned long value );
void                 lc_variant_set_boolean          ( lc_variant_t* p_variant, bool value );
void                 lc_variant_set_pointer          ( lc_variant_t* p_variant, const void* value );


#define    lc_variant_create_string( )                  lc_variant_create( LC_VARIANT_STRING )
#define    lc_variant_is_string( p_variant )            lc_variant_is_type( p_variant, LC_VARIANT_STRING )
#define    lc_variant_string( p_variant )               (lc_variant_value(p_variant).string)

#define    lc_variant_create_decimal( )                 lc_variant_create( LC_VARIANT_DECIMAL )
#define    lc_variant_is_decimal( p_variant )           lc_variant_is_type( p_variant, LC_VARIANT_DECIMAL )
#define    lc_variant_decimal( p_variant )              (lc_variant_value(p_variant).decimal)

#define    lc_variant_create_integer( )                 lc_variant_create( LC_VARIANT_INTEGER )
#define    lc_variant_is_integer( p_variant )           lc_variant_is_type( p_variant, LC_VARIANT_INTEGER )
#define    lc_variant_integer( p_variant )              (lc_variant_value(p_variant).integer)

#define    lc_variant_create_unsigned_integer( )        lc_variant_create( LC_VARIANT_UNSIGNED_INTEGER )
#define    lc_variant_is_unsigned_integer( p_variant )  lc_variant_is_type( p_variant, LC_VARIANT_UNSIGNED_INTEGER )
#define    lc_variant_unsigned_integer( p_variant )     (lc_variant_value(p_variant).unsigned_integer)

#define    lc_variant_create_boolean( )                 lc_variant_create( LC_VARIANT_BOOLEAN )
#define    lc_variant_is_boolean( p_variant )           lc_variant_is_type( p_variant, LC_VARIANT_BOOLEAN )
#define    lc_variant_boolean( p_variant )              (lc_variant_value(p_variant).boolean)

#define    lc_variant_create_pointer( )                 lc_variant_create( LC_VARIANT_POINTER )
#define    lc_variant_is_pointer( p_variant )           lc_variant_is_type( p_variant, LC_VARIANT_POINTER )
#define    lc_variant_pointer( p_variant )              (lc_variant_value(p_variant).pointer)

#ifdef __cplusplus
} /* external C linkage */
#endif
#endif /* _LC_VARIANT_H_ */
