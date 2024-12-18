#ifndef LUNA_VM_MEMORY_H
#define LUNA_VM_MEMORY_H

#include <cstdlib>

#include "common.h"

namespace Luna::VM {

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, old_count, new_count)                        \
  (type *)reallocate(pointer, sizeof(type) * (old_count),                      \
                     sizeof(type) * (new_count))

#define FREE_ARRAY(type, pointer, old_count)                                   \
  reallocate(pointer, sizeof(type) * (old_count), 0);

void *reallocate(void *pointer, size_t old_size, size_t new_size);

} // namespace Luna::VM

#endif
