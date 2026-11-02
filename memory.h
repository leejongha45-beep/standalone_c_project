#ifndef CP_MEMORY_H
#define CP_MEMORY_H

#include "types.h"

exret_t vm_reserve(size_t size, byte_t** out_target);
exret_t vm_commit(size_t size, byte_t* target);
exret_t vm_release(size_t size, byte_t* target);

#endif /* CP_MEMORY_H */
