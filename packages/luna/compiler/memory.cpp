#include "memory.h"

namespace Luna::VM {

void *reallocate(void *pointer, size_t old_size, size_t new_size) {
  if (new_size == 0) {
    free(pointer);

    return nullptr;
  }

  void *result = realloc(pointer, new_size);

  if (result == nullptr) {
    exit(1);
  }

  return result;
}

} // namespace Luna::VM
