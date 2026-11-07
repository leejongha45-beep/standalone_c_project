#include "sys_call.h"

int64_t arm64_syscall(int64_t sys_num, struct i64_span in) {
    register int64_t rx8 __asm__("x8") = sys_num;
    register int64_t rx0 __asm__("x0") = in.size > 0 ? in.data[0] : 0;
    register int64_t rx1 __asm__("x1") = in.size > 1 ? in.data[1] : 0;
    register int64_t rx2 __asm__("x2") = in.size > 2 ? in.data[2] : 0;
    register int64_t rx3 __asm__("x3") = in.size > 3 ? in.data[3] : 0;
    register int64_t rx4 __asm__("x4") = in.size > 4 ? in.data[4] : 0;
    register int64_t rx5 __asm__("x5") = in.size > 5 ? in.data[5] : 0;

    __asm__ volatile("svc #0"
                     : "+r"(rx0)
                     : "r"(rx8), "r"(rx1), "r"(rx2), "r"(rx3), "r"(rx4),
                       "r"(rx5)
                     : "memory", "cc");

    return rx0;
}
