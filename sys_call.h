#ifndef CP_SYS_CALL_H
#define CP_SYS_CALL_H

#include "types.h"

constexpr int64_t SYSCALL_READ = 63;
constexpr int64_t SYSCALL_WRITE = 64;
constexpr int64_t SYSCALL_EXIT = 93;
constexpr int64_t SYSCALL_EXIT_GROUP = 94;
constexpr int64_t SYSCALL_VM_UNMAP = 215;
constexpr int64_t SYSCALL_VM_MAP = 222;
constexpr int64_t SYSCALL_VM_ACCESS = 226;

int64_t arm64_syscall(int64_t sys_num, struct i64_span in);

#endif /* CP_SYS_CALL_H */
