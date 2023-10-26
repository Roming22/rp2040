#include "Memory.h"

extern "C" char *sbrk(int incr);

int Memory::freeMemory() {
  char top;
  return &top - reinterpret_cast<char *>(sbrk(0));
}

void Memory::printFreeMemory() { DEBUG_INFO("Memory: %d", freeMemory()); }