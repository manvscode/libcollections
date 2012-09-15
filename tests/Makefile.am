#AM_CFLAGS = $(DEPS_CFLAGS) -I ../src/ -L ../src/ -L .
AM_CFLAGS = -std=gnu99 -Wall -pedantic -g -ggdb -pg -O0 -D_DEBUG -DUNICODE -DUSE_ALLOCATORS -D_XOPEN_SOURCE -D_XOPEN_SOURCE_EXTENDED -I ../src/ -L ../src/ -L . 
LDADD     = -lcollections


bin_PROGRAMS = \
test-array \
test-bench-mark \
test-bheap \
test-bitset \
test-dlist \
test-flat-db \
test-hash-map \
test-hash-table \
test-lhash-table \
test-slist \
test-tstring \
test-rbtree \
test-variant \
test-vector

test_array_SOURCES       = test-array.c
test_bench_mark_SOURCES  = test-bench-mark.c
test_bheap_SOURCES       = test-bheap.c
test_bitset_SOURCES      = test-bitset.c
test_dlist_SOURCES       = test-dlist.c
test_flat_db_SOURCES     = test-flat-db.c
test_hash_map_SOURCES    = test-hash-map.c
test_hash_table_SOURCES  = test-hash-table.c
test_lhash_table_SOURCES = test-lhash-table.c
test_tstring_SOURCES     = test-tstring.c
test_slist_SOURCES       = test-slist.c
test_rbtree_SOURCES      = test-rbtree.c
test_vector_SOURCES      = test-vector.c
test_variant_SOURCES     = test-variant.c
