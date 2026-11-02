#include "memory.h"

#include "debug.h"
#include "sys_call.h"
#include "types.h"

exret_t vm_reserve(size_t size, byte_t** out_target) {
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

exret_t vm_commit(size_t size, byte_t* target) {
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

exret_t vm_release(size_t size,byte_t* target) {
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
