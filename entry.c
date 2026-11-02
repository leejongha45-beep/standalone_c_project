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
constexpr int64_t SYSCALL_VM_MAP = 222;
constexpr int64_t SYSCALL_VM_ACCESS = 226;

struct i64_span {
    const int64_t *data;
    size_t count;
};

static int64_t arm64_syscall(int64_t sys_num, struct i64_span in) {
    register int64_t rx8 __asm__("x8") = sys_num;
    register int64_t rx0 __asm__("x0") = in.count > 0 ? in.data[0] : 0;
    register int64_t rx1 __asm__("x1") = in.count > 1 ? in.data[1] : 0;
    register int64_t rx2 __asm__("x2") = in.count > 2 ? in.data[2] : 0;
    register int64_t rx3 __asm__("x3") = in.count > 3 ? in.data[3] : 0;
    register int64_t rx4 __asm__("x4") = in.count > 4 ? in.data[4] : 0;
    register int64_t rx5 __asm__("x5") = in.count > 5 ? in.data[5] : 0;

    __asm__ volatile("svc #0"
                     : "+r"(rx0)
                     : "r"(rx8), "r"(rx1), "r"(rx2), "r"(rx3), "r"(rx4),
                       "r"(rx5)
                     : "memory", "cc");

    return rx0;
}

[[noreturn]] void _start(void) {
    char ch;
    int64_t rin_read[3] = {0, (uptr_t)&ch, 1};

    int64_t n = arm64_syscall(SYSCALL_READ, (struct i64_span){rin_read, 3});
    if (n == 1) {
        int64_t rin_wirte[3] = {1, (uptr_t)&ch, 1};
        arm64_syscall(SYSCALL_WRITE, (struct i64_span){rin_wirte, 3});
    }

    int64_t rin_exit[3] = {0, 0, 0};
    arm64_syscall(SYSCALL_EXIT, (struct i64_span){rin_exit, 3});
    __builtin_unreachable();
}
