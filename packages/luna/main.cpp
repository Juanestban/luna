#include <cstdio>
#include <cstdlib>

#include "vm/vm.h"

using namespace Luna;

void repl(VM::VM *vm) {
  char line[1024];

  for (;;) {
    printf("> ");

    if (!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }

    vm->interpret(line);
  }
}

char *read_file(const char *path) {
  FILE *file = fopen(path, "rb");

  if (file == NULL) {
    fprintf(stderr, "Could not open file \"%s\".\n", path);
    exit(74);
  }

  fseek(file, 0L, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  char *buffer = (char *)malloc(file_size + 1);

  if (buffer == NULL) {
    fprintf(stderr, "Not enought memory to read \"%s\".\n", path);
    exit(74);
  }

  size_t bytes_read = fread(buffer, sizeof(char), file_size, file);

  if (bytes_read < file_size) {
    fprintf(stderr, "Could not read file \"%s\".\n", path);
    exit(74);
  }

  // EOF
  buffer[bytes_read] = '\0';

  fclose(file);

  return buffer;
}

static void run_file(const char *path, VM::VM *vm) {
  char *source = read_file(path);

  VM::InterpertResult result = vm->interpret(source);
  free(source);

  if (result == VM::INTERPRET_COMPILE_ERROR)
    exit(65);
  if (result == VM::INTERPRET_RUNTIME_ERROR)
    exit(70);
}

int main(int argc, const char *argv[]) {
  VM::VM *vm = new VM::VM();

  if (argc == 1) {
    repl(vm);
  } else if (argc == 2) {
    run_file(argv[1], vm);
  } else {
    fprintf(stderr, "Usage: luna [path]\n");
    exit(64);
  }

  vm->free_me();
  return 0;
}
