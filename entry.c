typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef unsigned long size_t;
typedef unsigned long uptr_t;

static_assert(sizeof(uint8_t) == 1);
static_assert(sizeof(uint16_t) == 2);
static_assert(sizeof(uint32_t) == 4);
static_assert(sizeof(uint64_t) == 8);

static_assert(sizeof(int8_t) == 1);
static_assert(sizeof(int16_t) == 2);
static_assert(sizeof(int32_t) == 4);
static_assert(sizeof(int64_t) == 8);

constexpr int64_t SYSCALL_READ = 63;
constexpr int64_t SYSCALL_WRITE = 64;
constexpr int64_t SYSCALL_EXIT = 93;

static int64_t arm64_syscall3(int64_t sys_num, int64_t in_x0, int64_t in_x1,
                              int64_t in_x2) {
    register int64_t rx8 __asm__("x8") = sys_num;
    register int64_t rx0 __asm__("x0") = in_x0;
    register int64_t rx1 __asm__("x1") = in_x1;
    register int64_t rx2 __asm__("x2") = in_x2;

    __asm__ volatile("svc #0"
                     : "+r"(rx0)
                     : "r"(rx8), "r"(rx1), "r"(rx2)
                     : "memory", "cc");

    return rx0;
}

[[noreturn]] void _start(void) {
    char ch;
    int64_t n = arm64_syscall3(SYSCALL_READ, 0, (uptr_t)&ch, 1);
    if (n == 1) {
        arm64_syscall3(SYSCALL_WRITE, 1, (uptr_t)&ch, 1);
    }

    arm64_syscall3(SYSCALL_EXIT, 0, 0, 0);
    __builtin_unreachable();
}
