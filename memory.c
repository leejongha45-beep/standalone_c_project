#include "memory.h"
#include "debug.h"
#include "int_math.h"
#include "sys_call.h"
#include "types.h"

exret_t virtual_memory_reserve(size_t size, byte_t** out_target) {
    CHECK(size > 0);
    CHECK(out_target != nullptr);
    CHECK(*out_target == nullptr);

    int64_t rin_reserve[6] = {0, size, 0, 0x22, -1, 0};
    const int64_t ex_addr =
        arm64_syscall(SYSCALL_VM_MAP, (struct i64_span){rin_reserve, 6});
    if (ex_addr < 0) {
        return EXRET_MEMORY_RESERVE_FAIL;
    }

    *out_target = (byte_t*)(uptr_t)ex_addr;

    return EXRET_SUCCESS;
}

exret_t virtual_memory_commit(size_t size, byte_t* target) {
    CHECK(size > 0);
    CHECK(target != nullptr);

    int64_t rin_commit[3] = {(uptr_t)target, size, 3};
    const int64_t result =
        arm64_syscall(SYSCALL_VM_ACCESS, (struct i64_span){rin_commit, 3});
    if (result < 0) {
        return EXRET_MEMORY_COMMIT_FAIL;
    }

    return EXRET_SUCCESS;
}

exret_t virtual_memory_release(size_t size, byte_t* target) {
    CHECK(size > 0);
    CHECK(target != nullptr);

    int64_t rin_release[2] = {(uptr_t)target, size};
    const int64_t result =
        arm64_syscall(SYSCALL_VM_UNMAP, (struct i64_span){rin_release, 2});
    if (result < 0) {
        return EXRET_MEMORY_RELEASE_FAIL;
    }

    return EXRET_SUCCESS;
}

exret_t virtual_memory_create(struct virtual_memory_create_info info,
                              struct virtual_memory* out_target) {
    CHECK(info.page_size != 0);
    CHECK(info.reserve_pages != 0);
    CHECK(info.commit_pages != 0);
    CHECK(info.reserve_pages >= info.commit_pages);
    CHECK(info.reserve_pages <= (size_t)-1 / info.page_size);
    CHECK(out_target != nullptr);
    CHECK(out_target->base == nullptr);

    byte_t* addr = nullptr;
    exret_t ex_base = virtual_memory_reserve(info.page_size * info.reserve_pages, &addr);
    if (ex_base != 0) {
        return ex_base;
    }
    CHECK(info.align == ALIGN_ANY || (uptr_t)addr % info.align == 0);

    exret_t commit_result = virtual_memory_commit(info.page_size * info.commit_pages, addr);
    if (commit_result != 0) {
        return commit_result;
    }

    out_target->base = addr;
    out_target->offset = 0;
    out_target->page_size = info.page_size;
    out_target->reserved_pages = info.reserve_pages;
    out_target->committed_pages = info.commit_pages;
    out_target->align = info.align;

    return EXRET_SUCCESS;
}

exret_t virtual_memroy_destroy(struct virtual_memory* alloc) {
    CHECK(alloc != nullptr);
    CHECK(alloc->base != nullptr);

    exret_t release_result =
        virtual_memory_release(alloc->reserved_pages * alloc->page_size, alloc->base);
    if (release_result != EXRET_SUCCESS) {
        return release_result;
    }

    alloc->base = nullptr;

    return EXRET_SUCCESS;
}

static inline uptr_t virtual_memory_offset_to_addr(const struct virtual_memory* mem) {
    return (uptr_t)mem->base + mem->offset;
}

exret_t unique_alloc_single_align(struct virtual_memory* mem,
                                  struct alloc_info info, void** out_target) {
    CHECK(mem != nullptr);
    CHECK(mem->base != nullptr);
    CHECK(mem->align != ALIGN_ANY);
    CHECK(info.size != 0);
    CHECK(info.size <= mem->reserved_pages * mem->page_size);
    CHECK(info.align != ALIGN_ANY);
    CHECK(mem->align % info.align == 0 || info.align % mem->align == 0);
    CHECK(out_target != nullptr);
    CHECK(*out_target == nullptr);

    const size_t reserved_size = mem->reserved_pages * mem->page_size;
    const size_t committed_size = mem->committed_pages * mem->page_size;
    const size_t align = info.align > mem->align ? info.align : mem->align;
    const size_t front_pad =
        virtual_memory_offset_to_addr(mem) % align == 0
            ? 0
            : align - virtual_memory_offset_to_addr(mem) % align;
    const size_t req_size = div_ceil(info.size, align) * align + front_pad;
    if (req_size > reserved_size - mem->offset) {
        return EXRET_ALLOCATE_NOT_ENOUGH_RESERVED_SIZE;
    }

    if (req_size > committed_size - mem->offset) {
        const size_t req_pages =
            div_ceil(req_size - (committed_size - mem->offset), mem->page_size);
        byte_t* taget_addr = mem->base + committed_size;
        const exret_t commit_result =
            virtual_memory_commit(req_pages * mem->page_size, taget_addr);
        if (commit_result != EXRET_SUCCESS) {
            return commit_result;
        }
        mem->committed_pages += req_pages;
    }

    *out_target = mem->base + mem->offset + front_pad;
    mem->offset += req_size;

    return EXRET_SUCCESS;
}
