#ifndef CP_MEMORY_H
#define CP_MEMORY_H

#include "types.h"

typedef enum : size_t {
    ALIGN_ANY = 0,
    ALIGN_1 = 1,
    ALIGN_2 = 2,
    ALIGN_4 = 4,
    ALIGN_8 = 8,
    ALIGN_16 = 16,
    ALIGN_32 = 32,
    ALIGN_64 = 64,
} align_t;

struct virtual_memory_create_info {
    size_t page_size;
    size_t reserve_pages;
    size_t commit_pages;
    align_t align;
};

struct virtual_memory {
    byte_t* base;
    size_t offset;
    size_t page_size;
    size_t reserved_pages;
    size_t committed_pages;
    align_t align;
};

struct alloc_info {
    size_t size;
    align_t align;
};

exret_t virtual_memory_reserve(size_t size, byte_t** out_target);
exret_t virtual_memory_commit(size_t size, byte_t* target);
exret_t virtual_memory_release(size_t size, byte_t* target);

exret_t virtual_memory_create(struct virtual_memory_create_info info,
                              struct virtual_memory* out_target);
exret_t virtual_memroy_destroy(struct virtual_memory* alloc);
exret_t unique_alloc_single_align(struct virtual_memory* mem,
                                  struct alloc_info info, void** out_target);

#endif /* CP_MEMORY_H */
