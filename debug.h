#ifndef CP_DEBUG_H
#define CP_DEBUG_H

#if defined(CP_DEBUG) && CP_DEBUG

#include "exit.h" // IWYU pragma: keep

#define CHECK(condition)             \
    do {                             \
        if (!(condition)) {          \
            exit(EXIT_CHECK_FAILED); \
        }                            \
    } while (0)

#else

#define CHECK(condition) ((void)0)

#endif

#endif /* CP_DEBUG_H */
