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
#ifndef _LC_BITSET_H_
#define _LC_BITSET_H_
/**
 * @file bitset.h
 * @brief A collection of bits.
 *
 * @defgroup lc_bitset Bitset
 * @ingroup Collections
 * @{
 */
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct lc_bitset {
	unsigned char*  array;
	size_t bit_size;
} lc_bitset_t;

#if CHAR_BIT == 8 /* use shifts when possible */
#define lc_bits_to_bytes(bits)                (((bits) + 7) >> 3)
#define lc_bit_to_index(bit)                  ((bit) >> 3)
#else
#define lc_bits_to_bytes(bits)                (((bits) + CHAR_BIT - 1) / CHAR_BIT)
#define lc_bit_to_index(bit)                  ((bit) / CHAR_BIT )
#endif


bool     lc_bitset_create  ( lc_bitset_t* p_bitset, size_t bits );
void     lc_bitset_destroy ( lc_bitset_t* p_bitset );
void     lc_bitset_clear   ( lc_bitset_t* p_bitset );
bool     lc_bitset_resize  ( lc_bitset_t* p_bitset, size_t bits );
char*    lc_bitset_string  ( lc_bitset_t* p_bitset );
#define  lc_bitset_bits(p_bitset)              ((p_bitset)->bit_size)

#if !defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L)
void lc_bitset_set     ( lc_bitset_t* p_bitset, size_t bit );
void lc_bitset_unset   ( lc_bitset_t* p_bitset, size_t bit );
bool lc_bitset_test    ( const lc_bitset_t* p_bitset, size_t bit );
#else
static inline void lc_bitset_set( lc_bitset_t* p_bitset, size_t bit )
{
	assert( p_bitset );
	assert( bit < p_bitset->bit_size );
	p_bitset->array[ lc_bit_to_index(bit) ] |= (0x01 << (bit % CHAR_BIT));
}

static inline void lc_bitset_unset( lc_bitset_t* p_bitset, size_t bit )
{
	assert( p_bitset );
	assert( bit < p_bitset->bit_size );
	p_bitset->array[ lc_bit_to_index(bit) ] &= ~(0x01 << (bit % CHAR_BIT));
}

static inline bool lc_bitset_test( const lc_bitset_t* p_bitset, size_t bit )
{
	assert( p_bitset );
	return p_bitset->array[ lc_bit_to_index(bit) ] & (0x01 << (bit % CHAR_BIT));
}
#endif

#ifdef __cplusplus
} /* external C linkage */
#endif
#endif /* _LC_BITSET_H_ */
