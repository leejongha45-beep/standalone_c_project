#include "exit.h"
#include "sys_call.h"
#include "types.h"

[[noreturn]] void exit(enum exit_code code) {
    int64_t status = (int64_t)code;
    arm64_syscall(SYSCALL_EXIT_GROUP, (struct i64_span){&status, 1});
    __builtin_unreachable();
}
