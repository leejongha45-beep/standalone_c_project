#include "exit.h"
#include "memory.h"
#include "sys_call.h"
#include "types.h"

void print(const char* msg, ...) {
    char buf[128];
    size_t len = 0;

    __builtin_va_list args;
    __builtin_c23_va_start(args);

    while (*msg != '\0' && len < sizeof(buf)) {
        if (msg[0] != '%' || msg[1] != 'd') {
            buf[len++] = *msg++;
            continue;
        }

        msg += 2;
        int64_t val = __builtin_va_arg(args, int);
        if (val < 0) {
            buf[len++] = '-';
            val = -val;
        }

        char digits[10];
        size_t count = 0;
        do {
            digits[count++] = (char)('0' + val % 10);
            val /= 10;
        } while (val != 0);

        while (count > 0 && len < sizeof(buf)) {
            buf[len++] = digits[--count];
        }
    }

    __builtin_va_end(args);

    const string_t str = {.data = buf, .len = len};
    const int64_t rin_write[3] = {
        2,
        (int64_t)(uptr_t)str.data,
        (int64_t)str.len,
    };
    (void)arm64_syscall(SYSCALL_WRITE, (struct i64_span){rin_write, 3});
}

[[noreturn]] void _start(void) {
    struct virtual_memory log_vm;

    exit(0);
    __builtin_unreachable();
}
