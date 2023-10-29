#include "Memory.h"

extern "C" char *sbrk(int incr);

int Memory::GetFreeMemory() {
  char top;
  return &top - reinterpret_cast<char *>(sbrk(0));
}

void Memory::PrintMemoryUsage() {
  int memoryUsed = totalMemory - GetFreeMemory();
  float percentUsed = 100.0 * (float)memoryUsed / totalMemory;
  DEBUG_INFO("Memory: %d (%.2f%%)", memoryUsed, percentUsed);
}

int Memory::totalMemory = Memory::GetFreeMemory();