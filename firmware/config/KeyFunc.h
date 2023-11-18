#ifndef MYMK_CONFIG_KEYFUNC
#define MYMK_CONFIG_KEYFUNC

#include "../logic/quantum/Timeline.h"

#include <functional>
#include <map>
#include <string>

namespace config {
typedef std::function<void(logic::quantum::Timeline &,
                           const std::string &switch_uid)>
    KeyFunc;
} // namespace config
#endif