#ifndef MYMK_UTILS_MEMORY
#define MYMK_UTILS_MEMORY

#include "Debug.hpp"

class Memory {
protected:
  static int totalMemory;

public:
  static int GetFreeMemory();
  static void PrintMemoryUsage();
};
#endif