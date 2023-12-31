#include "firmware/utils/Debug.hpp"
#include "firmware/config/key/Chord.h"
#include "firmware/config/key/Keycode.h"
#include "firmware/config/key/Layer.h"
#include "firmware/config/key/TapHold.h"
#include "firmware/config/loader/Chord.h"
#include "firmware/config/loader/Keyboard.h"
#include "firmware/config/loader/Key.h"
#include "firmware/config/loader/Layer.h"
#include "firmware/hardware/board/BaseBoard.h"
#include "firmware/hardware/board/DaughterBoard.h"
#include "firmware/hardware/board/MotherBoard.h"
#include "firmware/hardware/key/KeyMatrix.h"
#include "firmware/hardware/key/KeySwitch.h"
#include "firmware/hardware/led/Pixels.h"
#include "firmware/hardware/txrx/BitBang.h"
#include "firmware/hardware/usb/Key.h"
#include "firmware/hardware/usb/Mouse.h"
#include "firmware/Keyboard.h"
#include "firmware/logic/Events.h"
#include "firmware/logic/feature/Chord.h"
#include "firmware/logic/feature/Keycode.h"
#include "firmware/logic/feature/Key.h"
#include "firmware/logic/feature/Layer.h"
#include "firmware/logic/feature/TapHold.h"
#include "firmware/logic/ObjectManager.h"
#include "firmware/logic/quantum/Timeline.h"
#include "firmware/logic/quantum/Universe.h"
#include "firmware/logic/Timer.h"
#include "firmware/logic/typedef.h"
#include "firmware/utils/Fps.h"
#include "firmware/utils/Memory.h"
#include "firmware/utils/Time.h"
#include "firmware/config/key/Chord.cpp"
#include "firmware/config/key/Keycode.cpp"
#include "firmware/config/key/Layer.cpp"
#include "firmware/config/key/TapHold.cpp"
#include "firmware/config/loader/Chord.cpp"
#include "firmware/config/loader/Keyboard.cpp"
#include "firmware/config/loader/Key.cpp"
#include "firmware/config/loader/Layer.cpp"
#include "firmware/hardware/board/BaseBoard.cpp"
#include "firmware/hardware/board/DaughterBoard.cpp"
#include "firmware/hardware/board/MotherBoard.cpp"
#include "firmware/hardware/key/KeyMatrix.cpp"
#include "firmware/hardware/key/KeySwitch.cpp"
#include "firmware/hardware/led/Pixels.cpp"
#include "firmware/hardware/txrx/BitBang.cpp"
#include "firmware/hardware/usb/Key.cpp"
#include "firmware/hardware/usb/Mouse.cpp"
#include "firmware/Keyboard.cpp"
#include "firmware/logic/Events.cpp"
#include "firmware/logic/feature/Chord.cpp"
#include "firmware/logic/feature/Keycode.cpp"
#include "firmware/logic/feature/Key.cpp"
#include "firmware/logic/feature/Layer.cpp"
#include "firmware/logic/feature/TapHold.cpp"
#include "firmware/logic/ObjectManager.cpp"
#include "firmware/logic/quantum/Timeline.cpp"
#include "firmware/logic/quantum/Universe.cpp"
#include "firmware/logic/Timer.cpp"
#include "firmware/utils/Fps.cpp"
#include "firmware/utils/Memory.cpp"
#include "firmware/utils/Time.cpp"
