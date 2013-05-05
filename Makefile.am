SUBDIRS = src tests

Android.mk: Makefile.am
	@androgenizer -:PROJECT libcollections \
	-:REL_TOP $(top_srcdir)/src -:ABS_TOP $(abs_top_srcdir)/src \
	-:SHARED libcollections \
	-:SOURCES \
	src/array.c \
	src/bheap.c \
	src/dlist.c \
	src/hash-functions.c \
	src/hash-table.c \
	src/rbtree.c \
	src/tree-map.c \
	src/variant.c \
	src/bench-mark.c \
	src/bitset.c \
	src/flat-db.c \
	src/hash-map.c \
	src/lhash-table.c \
	src/slist.c \
	src/tstring.c \
	src/vector.c \
	-I./src/ \
	-include ./src/libcollections-config.h \
	-:CFLAGS -std=c99 -Wall -O3 \
	> $@

android: Android.mk
	ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk NDK_OUT=./lib

android-clean:
	@ndk-build clean NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk NDK_OUT=./lib
	echo "Removing Android.mk"; rm -f Android.mk
