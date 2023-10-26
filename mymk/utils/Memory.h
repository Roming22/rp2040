#ifndef MYMK_UTILS_MEMORY
#define MYMK_UTILS_MEMORY

#include "Debug.hpp"

class Memory {
public:
  static int freeMemory();
  static void printFreeMemory();
};
#endif