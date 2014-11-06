// public domain

#include <iostream>
#include "testcpp.h"


extern "C"
int entry(int argc, char* argv[]) {
  
  if (argc > 0) {
    std::cout << "cpp callback: " << cppCallout(argv[0]);
  }
  return 0;
}
