if ENABLE_TESTS
AM_CFLAGS = -std=c99 -pedantic -g -ggdb -pg -O0 -D_DEBUG -DUNICODE -D_XOPEN_SOURCE -D_XOPEN_SOURCE_EXTENDED -I ../src/
LDADD     = -lm $(top_builddir)/lib/.libs/libcollections.a


bin_PROGRAMS = \
$(top_builddir)/bin/example-array \
$(top_builddir)/bin/example-benchmark \
$(top_builddir)/bin/example-bitset \
$(top_builddir)/bin/example-dlist \
$(top_builddir)/bin/example-flat-db \
$(top_builddir)/bin/example-vector \
$(top_builddir)/bin/example-hash-map \
$(top_builddir)/bin/example-hash-table \
$(top_builddir)/bin/example-lhash-table \
$(top_builddir)/bin/example-binary-heap \
$(top_builddir)/bin/example-slist \
$(top_builddir)/bin/example-lc-string \
$(top_builddir)/bin/example-rbtree \
$(top_builddir)/bin/example-tree-map \
$(top_builddir)/bin/example-variant

__top_builddir__bin_example_array_SOURCES       = example-array.c
__top_builddir__bin_example_benchmark_SOURCES   = example-benchmark.c
__top_builddir__bin_example_bitset_SOURCES      = example-bitset.c
__top_builddir__bin_example_dlist_SOURCES       = example-dlist.c
__top_builddir__bin_example_flat_db_SOURCES     = example-flat-db.c
__top_builddir__bin_example_vector_SOURCES      = example-vector.c
__top_builddir__bin_example_hash_map_SOURCES    = example-hash-map.c
__top_builddir__bin_example_hash_table_SOURCES  = example-hash-table.c
__top_builddir__bin_example_lhash_table_SOURCES = example-lhash-table.c
__top_builddir__bin_example_binary_heap_SOURCES = example-binary-heap.c
__top_builddir__bin_example_lc_string_SOURCES   = example-lc-string.c
__top_builddir__bin_example_slist_SOURCES       = example-slist.c
__top_builddir__bin_example_rbtree_SOURCES      = example-rbtree.c
__top_builddir__bin_example_tree_map_SOURCES    = example-tree-map.c
__top_builddir__bin_example_variant_SOURCES     = example-variant.c
endif
