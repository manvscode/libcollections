LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

SRC_PATH         := ../../src
LOCAL_MODULE     := libcollections
LOCAL_C_INCLUDES := $(SRC_PATH)
LOCAL_SRC_FILES  := \
	$(SRC_PATH)/bitset.c \
	$(SRC_PATH)/buffer.c \
	$(SRC_PATH)/dlist.c \
	$(SRC_PATH)/flat-db.c \
	$(SRC_PATH)/hash-functions.c \
	$(SRC_PATH)/hash-map.c \
	$(SRC_PATH)/hash-table.c \
	$(SRC_PATH)/lhash-table.c \
	$(SRC_PATH)/rbtree.c \
	$(SRC_PATH)/slist.c \
	$(SRC_PATH)/tree-map.c \
	$(SRC_PATH)/tstring.c \
	$(SRC_PATH)/variant.c \
	$(SRC_PATH)/vector.c

include $(BUILD_STATIC_LIBRARY)
