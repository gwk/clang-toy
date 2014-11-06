// public domain

#include <stdio.h>
#include "testc.h"


int entry(int argc, char* argv[]) {
  if (argc > 0) {
    printf("c callback: %s", cCallout(argv[0]));
  }
  return 1;
}
