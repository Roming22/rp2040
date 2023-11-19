#ifndef MYMK_LOGIC_TYPEDEF
#define MYMK_LOGIC_TYPEDEF

#include <functional>
#include <string>

namespace logic {
namespace quantum {
class Timeline;
} // namespace quantum

typedef std::function<void(logic::quantum::Timeline &)> ActionFunc;
typedef std::function<void(logic::quantum::Timeline &,
                           const std::string &switch_uid)>
    KeyFunc;
} // namespace logic
#endif