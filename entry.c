#include "sys_call.h"
#include "exit.h"
#include "types.h"

[[noreturn]] void _start(void) {
    char ch;
    int64_t rin_read[3] = {0, (uptr_t)&ch, 1};

    int64_t n = arm64_syscall(SYSCALL_READ, (struct i64_span){rin_read, 3});
    if (n == 1) {
        int64_t rin_wirte[3] = {1, (uptr_t)&ch, 1};
        arm64_syscall(SYSCALL_WRITE, (struct i64_span){rin_wirte, 3});
    }

    exit(0);
    __builtin_unreachable();
}
