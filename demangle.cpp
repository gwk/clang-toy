#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cxxabi.h>

#define errFL(fmt, ...) fprintf(stderr, fmt, ## __VA_ARGS__)

// write an error message and then exit.
#define error(fmt, ...) { \
  errFL("error: " fmt, ## __VA_ARGS__); \
  exit(1); \
}

// check that a condition is true; otherwise error.
#define check(condition, fmt, ...) { if (!(condition)) error(fmt, ## __VA_ARGS__); }

size_t buffer_len = 0;
char* buffer = NULL;

void demangle(const char* name) {
  int status;
  // OSX adds leading underscore to symbols, which must be removed.
  // TODO: add logic for per supported OS.
#if __APPLE__
  if (*name) {
    name++;
  }
#endif
  // this api will realloc as necessary.
  buffer = abi::__cxa_demangle(name, buffer, &buffer_len, &status);
  switch (status) {
    case 0: return;
    case -1: error("memory allocation failure.");
    case -2: {
      int len = strlen(name) + 1;
      if (buffer_len < len) {
        buffer_len = len * 2;
        buffer = (char*)realloc(buffer, buffer_len);
      }
      strcpy(buffer, name);
      return;
    }
    case -3: error("invalid argument");
    default: error("unknown error: %d", status);
  }
}


int main(int argc, const char * argv[]) {
  //buffer_len = 1024;
  //buffer = (char*)malloc(buffer_len);
  for (int i = 1; i < argc; i++) {
    const char* mangled = argv[i];
    demangle(mangled);
    printf("%s -> %s\n", mangled, buffer);
  }
  return 0;
}
