#ifndef CP_TYPES_H
#define CP_TYPES_H

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef __SIZE_TYPE__ size_t;
typedef __UINTPTR_TYPE__ uptr_t;
typedef unsigned char byte_t;

typedef enum : uint16_t {
    EXRET_SUCCESS = 0,
    EXRET_MEMORY_RESERVE_FAIL = 100,
    EXRET_MEMORY_COMMIT_FAIL = 101,
    EXRET_MEMORY_RELEASE_FAIL = 102,
    EXRET_ALLOCATE_NOT_ENOUGH_RESERVED_SIZE = 200,
} exret_t;

static_assert(sizeof(uint8_t) == 1);
static_assert(sizeof(uint16_t) == 2);
static_assert(sizeof(uint32_t) == 4);
static_assert(sizeof(uint64_t) == 8);

static_assert(sizeof(int8_t) == 1);
static_assert(sizeof(int16_t) == 2);
static_assert(sizeof(int32_t) == 4);
static_assert(sizeof(int64_t) == 8);

static_assert(sizeof(uptr_t) == sizeof(void*));
static_assert((size_t)-1 > (size_t)0);

typedef struct {
    char* data;
    size_t len;
} string_t;

struct i64_span {
    const int64_t* data;
    size_t size;
};

struct byte_buffer {
    byte_t* start;
    size_t size;
};

#endif /* CP_TYPES_H */
