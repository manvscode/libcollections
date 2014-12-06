if ENABLE_TESTS
AM_CFLAGS = -std=c99 -pedantic -g -ggdb -pg -O0 -D_DEBUG -DUNICODE -D_XOPEN_SOURCE -D_XOPEN_SOURCE_EXTENDED -I ../src/
LDADD     = -lm $(top_builddir)/lib/.libs/libcollections.a


bin_PROGRAMS = \
$(top_builddir)/bin/test-array \
$(top_builddir)/bin/test-bench-mark \
$(top_builddir)/bin/test-bitset \
$(top_builddir)/bin/test-dlist \
$(top_builddir)/bin/test-vector \
$(top_builddir)/bin/test-flat-db \
$(top_builddir)/bin/test-hash-map \
$(top_builddir)/bin/test-hash-table \
$(top_builddir)/bin/test-lhash-table \
$(top_builddir)/bin/test-binary-heap \
$(top_builddir)/bin/test-slist \
$(top_builddir)/bin/test-tstring \
$(top_builddir)/bin/test-rbtree \
$(top_builddir)/bin/test-tree-map \
$(top_builddir)/bin/test-variant 

__top_builddir__bin_test_array_SOURCES       = test-array.c
__top_builddir__bin_test_bench_mark_SOURCES  = test-bench-mark.c
__top_builddir__bin_test_bitset_SOURCES      = test-bitset.c
__top_builddir__bin_test_dlist_SOURCES       = test-dlist.c
__top_builddir__bin_test_vector_SOURCES      = test-vector.c
__top_builddir__bin_test_flat_db_SOURCES     = test-flat-db.c
__top_builddir__bin_test_hash_map_SOURCES    = test-hash-map.c
__top_builddir__bin_test_hash_table_SOURCES  = test-hash-table.c
__top_builddir__bin_test_lhash_table_SOURCES = test-lhash-table.c
__top_builddir__bin_test_binary_heap_SOURCES = test-binary-heap.c
__top_builddir__bin_test_tstring_SOURCES     = test-tstring.c
__top_builddir__bin_test_slist_SOURCES       = test-slist.c
__top_builddir__bin_test_rbtree_SOURCES      = test-rbtree.c
__top_builddir__bin_test_tree_map_SOURCES    = test-tree-map.c
__top_builddir__bin_test_variant_SOURCES     = test-variant.c
endif
