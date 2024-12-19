#include "memory.h"
#include <cstdlib>
#include <cstring>

namespace Luna::Compiler {

void *reallocate(void *pointer, size_t old_size, size_t new_size) {
  if (new_size == 0) {
    free(pointer);

    return nullptr;
  }

  if (pointer == nullptr) {
    void *result = malloc(new_size);

    if (result == nullptr) {
      return nullptr;
    }

    return result;
  }

  if (new_size > old_size) {
    void *result = malloc(new_size);

    if (result == nullptr) {
      return nullptr;
    }

    memcpy(result, pointer, old_size);
    free(pointer);
    memset(static_cast<char *>(result) + old_size, 0, new_size - old_size);

    return result;
  }

  void *result = realloc(pointer, new_size);

  if (result == nullptr) {
    exit(1);
  }

  return result;
}

} // namespace Luna::Compiler
