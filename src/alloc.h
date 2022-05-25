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
#ifndef _LC_ALLOC_H_
#define _LC_ALLOC_H_

#include <stdlib.h>

/**
 * @file alloc.h
 * @brief Memory allocation and deallocation interface functions and macros.
 * @defgroup lc_alloc Memory Allocation
 * @ingroup Memory
 * @{
 */

/**
 * lc_alloc_fxn_t is a function pointer for a memory allocation function.
 * It's signature matches the standard C library malloc() function and
 * allows data structures to be allocated with a custom allocation function.
 */
typedef void* (*lc_alloc_fxn_t) ( size_t size );

/**
 * lc_free_fxn_t is a function pointer for a memory deallocation function.
 * It's signature matches the standard C library free() function and allows
 * data structures to be allocated with a custom allocation function.
 */
typedef void  (*lc_free_fxn_t)  ( void *data );

/**
 * lc_alloc_type(type) is a macro for allocating enough memory for the type
 * passed to lc_alloc_type().
 */
#define lc_alloc_type( type )      ((type *) malloc( sizeof(type) ))
/**
 * lc_free_type(ptr) is a macro for deallocating memory for the pointer
 * passed to lc_free_type().
 */
#define lc_free_type( ptr )        (free( ptr ))

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define lc_construct( var, ... )   var##_initialize( __VA_ARGS__ )
#define lc_destruct( var, ... )    var##_deinitialize( __VA_ARGS__ )
#endif


#endif /* _LC_ALLOC_H_ */
