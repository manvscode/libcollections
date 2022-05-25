# collections
The collections library is a set of data structures, types, and utility code for C programs. It was designed to be developer friendly, efficient, and versatile.

## Building

### Linux
1. `autoreconf -i`
2. `./configure`
3. `make`
4. `make install`

### Cross-compiling for Windows with MinGW

1. `autoreconf -i`
2. Run the configure script for the host architecture:
* For x86: `./configure --host=i686-w64-mingw32`
* For x86_64: `./configure --host=x86_64-w64-mingw32`
3. `make`
4. `make install`

## Supported Data Structures
 * Array
 * Binary Heap
 * Bit Set
 * Doubly Linked List
 * Singly Linked List
 * Hash Table
 * Linear Open Addressing Hash Table
 * Hash Map
 * Binary Flat Files
 * Red Black Tree
 * Tree Map
 * Vector (i.e. growable array)

## Supported Types and Utilities
 * Bench Marking
 * Common Hash Functions
 * Common Macros
 * Wrapper for Unicode and ASCII strings
 * Variant Data Type
 * Common Types

## OS Support
 * Linux
 * Mac OS X
 * iOS
 * Android
 * Microsoft Windows Vista using Visual Studio 2008 (limited support); later versions are untested.

## License
    Copyright (C) 2010-2022 by Joseph A. Marrero.  https://joemarrero.com/
    
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
