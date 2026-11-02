#ifndef CP_EXIT_H
#define CP_EXIT_H

#include "types.h"

enum exit_code : int16_t
{
    EXIT_SUCCESS = 0,
    EXIT_CHECK_FAILED = -1,
};

[[noreturn]] void exit(enum exit_code status);

#endif /* CP_EXIT_H */
