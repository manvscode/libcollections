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
#ifndef _TYPES_H_
#define _TYPES_H_

#include <stddef.h>
#include <wchar.h>

#ifndef NULL
	#define NULL ((void *) 0)
#endif

#ifndef FALSE
	#define FALSE 0
#endif
#ifndef TRUE
	#define TRUE  1 
#endif



#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
	#include <stdint.h>

	typedef int8_t    int8;
	typedef uint8_t   uint8;
	typedef int16_t   int16;
	typedef uint16_t  uint16;
	typedef int32_t   int32;
	typedef uint32_t  uint32;
	typedef int64_t   int64;
	typedef uint64_t  uint64;
	typedef wchar_t   wchar;
	//typedef bool      boolean; /* FALSE = 0, otherwise TRUE */
	#define __inline   inline
#else /* Not C99 compliant */
	typedef signed char         int8;
	typedef unsigned char       uint8;
	typedef signed short        int16;
	typedef unsigned short      uint16;
	typedef signed int          int32;
	typedef unsigned int        uint32;
	/*
	typedef signed long long    int64;
	typedef unsigned long long  uint64; 
	*/
	//typedef char                boolean; /* FALSE = 0, otherwise TRUE */
	#define __inline  
#endif

#ifndef boolean
typedef char                boolean; /* FALSE = 0, otherwise TRUE */
#endif
#ifndef byte
typedef unsigned char  byte;
#endif
typedef uint16         ushort;
typedef uint32         uint;
#ifdef ulong
typedef uint64         ulong;
#endif


typedef uint32         ui32;
typedef int32          i32;
typedef uint16         ui16;
typedef int16          i16;
typedef float          f32;
typedef double         f64;
typedef unsigned char  uc8;

#endif /* _TYPES_H_ */
