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

typedef unsigned long size_t;
typedef unsigned long long uptr_t;
typedef unsigned char byte_t;

typedef enum : uint16_t {
    EXRET_SUCCESS = 0,
    EXRET_MEMORY_RESERVE_FAIL = 100,
    EXRET_MEMORY_COMMIT_FAIL = 101,
    EXRET_MEMORY_RELEASE_FAIL = 102,
} exret_t;

static_assert(sizeof(uint8_t) == 1);
static_assert(sizeof(uint16_t) == 2);
static_assert(sizeof(uint32_t) == 4);
static_assert(sizeof(uint64_t) == 8);

static_assert(sizeof(int8_t) == 1);
static_assert(sizeof(int16_t) == 2);
static_assert(sizeof(int32_t) == 4);
static_assert(sizeof(int64_t) == 8);

struct i64_span {
    const int64_t *data;
    size_t count;
};

#endif /* CP_TYPES_H */

