
# Add new files in alphabetical order. Thanks.
libcollections_src = \
array.c \
bitset.c \
buffer.c \
dlist.c \
hash-functions.c \
hash-map.c \
hash-table.c \
lc-string.c \
lhash-table.c \
rbtree.c \
slist.c \
textbuffer.c \
tree-map.c \
variant.c \
vector.c

# Add new files in alphabetical order. Thanks.
libcollections_headers = \
alloc.h \
array.h \
benchmark.h \
binary-heap.h \
bitset.h \
buffer.h \
dlist.h \
hash-functions.h \
hash-map.h \
hash-table.h \
heap.h \
lc-string.h \
lhash-table.h \
libcollections-config.h \
macros.h \
pool.h \
rbtree.h \
slist.h \
textbuffer.h \
tree-map.h \
variant.h \
vector.h

if WINDOWS
# Intentionally empty; Flat DB is not supported on Windows yet.
    libcollections_src += benchmark-windows.c
else # Linux and MacOS X
    libcollections_src += flat-db.c benchmark-unix.c
    libcollections_headers += flat-db.h
endif


library_includedir      = $(includedir)/@PACKAGE_NAME@-@PACKAGE_VERSION@/collections/
library_include_HEADERS = $(libcollections_headers)

# Library
lib_LTLIBRARIES                               = $(top_builddir)/lib/libcollections.la
__top_builddir__lib_libcollections_la_SOURCES = $(libcollections_src)
__top_builddir__lib_libcollections_la_CFLAGS  = -fPIC
__top_builddir__lib_libcollections_la_LDFLAGS = --no-undefined
__top_builddir__lib_libcollections_la_LIBADD  = -lm

