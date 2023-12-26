#ifndef MYMK_UTILS_DEBUG
#define MYMK_UTILS_DEBUG
#include "Arduino_DebugUtils.h"

#ifndef DEBUG_ENABLED
#undef DEBUG_ERROR
#undef DEBUG_WARNING
#undef DEBUG_INFO
#undef DEBUG_DEBUG
#undef DEBUG_VERBOSE
#endif

#ifndef DEBUG_ERROR
#define DEBUG_ERROR(...) ((void)0)
#endif
#ifndef DEBUG_WARNING
#define DEBUG_WARNING(...) ((void)0)
#endif
#ifndef DEBUG_INFO
#define DEBUG_INFO(...) ((void)0)
#endif
#ifndef DEBUG_DEBUG
#define DEBUG_DEBUG(...) ((void)0)
#endif
#ifndef DEBUG_VERBOSE
#define DEBUG_VERBOSE(...) ((void)0)
#endif
#endif