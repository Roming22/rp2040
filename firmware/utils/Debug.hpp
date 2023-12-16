#ifndef MYMK_UTILS_DEBUG
#define MYMK_UTILS_DEBUG
#include "Arduino_DebugUtils.h"

#ifndef DEBUG_ERROR
#define DEBUG_ERROR(...)                                                       \
  do {                                                                         \
  } while (0)
#endif
#ifndef DEBUG_INFO
#define DEBUG_INFO(...)                                                        \
  do {                                                                         \
  } while (0)
#endif
#ifndef DEBUG_DEBUG
#define DEBUG_DEBUG(...)                                                       \
  do {                                                                         \
  } while (0)
#endif
#ifndef DEBUG_VERBOSE
#define DEBUG_VERBOSE(...)                                                     \
  do {                                                                         \
  } while (0)
#endif
#endif