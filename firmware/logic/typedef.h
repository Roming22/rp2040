#ifndef MYMK_LOGIC_TYPEDEF
#define MYMK_LOGIC_TYPEDEF

#include <functional>
#include <memory>
#include <string>

namespace logic {
namespace quantum {
class Timeline;
} // namespace quantum

typedef std::function<void(logic::quantum::Timeline &)> ActionFunc;
typedef std::shared_ptr<ActionFunc> ActionFuncPtr;
ActionFuncPtr ActionFuncNoOp =
    std::make_shared<ActionFunc>([](logic::quantum::Timeline &) {});

typedef std::function<void(logic::quantum::Timeline &,
                           const std::string &switch_uid)>
    KeyFunc;
} // namespace logic
#endif