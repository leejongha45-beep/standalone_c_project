#include "int_math.h"
#include "debug.h"
#include "types.h"

inline size_t div_ceil(size_t dividend, size_t divisor) {
    CHECK(divisor != 0);

    return dividend / divisor + (dividend % divisor != 0);
}
