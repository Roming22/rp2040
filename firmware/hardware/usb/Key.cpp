#include "Key.h"

#include <Keyboard.h>

namespace hardware {
namespace usb {
Key::Key(const char key) : keycode(key), count(0) {}

void Key::Init() {
  DEBUG_VERBOSE("hardware::usb::Key:Init");
  for (auto pair : definition2code) {
    if (Key::map.count(pair.second) == 0) {
      Key::map[pair.second] = new Key(pair.second);
    } else {
      DEBUG_DEBUG("%s is a duplicate", pair.first.c_str());
    }
  }
  DEBUG_DEBUG("Registered keycodes: %d", Key::map.size());

  // Init the USB keyboard
  Keyboard.begin();
}

void Key::Press(const std::string key_definition) {
  DEBUG_VERBOSE("hardware::usb::Key:Press");
  if (key_definition == "NONE") {
    return;
  }
  Key &event = *(map[definition2code[key_definition]]);
  if (event.count == 0) {
    DEBUG_INFO("Key::Press %s", key_definition.c_str());
    Keyboard.press(event.keycode);
  }
  event.count += 1;
}

void Key::Release(const std::string key_definition) {
  DEBUG_VERBOSE("hardware::usb::Key:Release");
  if (key_definition == "NONE") {
    return;
  }
  Key &event = *(map[definition2code[key_definition]]);
  event.count -= 1;
  if (event.count == 0) {
    DEBUG_INFO("hardware::usb::Key::Release %s", key_definition.c_str());
    Keyboard.release(event.keycode);
  }
}

std::map<const char, Key *> Key::map;
std::map<const std::string, const char> Key::definition2code = {
    // Alphas
    {"A", 'a'},
    {"B", 'b'},
    {"C", 'c'},
    {"D", 'd'},
    {"E", 'e'},
    {"F", 'f'},
    {"G", 'g'},
    {"H", 'h'},
    {"I", 'i'},
    {"J", 'j'},
    {"K", 'k'},
    {"L", 'l'},
    {"M", 'm'},
    {"N", 'n'},
    {"O", 'o'},
    {"P", 'p'},
    {"Q", 'q'},
    {"R", 'r'},
    {"S", 's'},
    {"T", 't'},
    {"U", 'u'},
    {"V", 'v'},
    {"W", 'w'},
    {"X", 'x'},
    {"Y", 'y'},
    {"Z", 'z'},
    {"SPACE", ' '},
    // Numbers
    {"0", '0'},
    {"1", '1'},
    {"2", '2'},
    {"3", '3'},
    {"4", '4'},
    {"5", '5'},
    {"6", '6'},
    {"7", '7'},
    {"8", '8'},
    {"9", '9'},
    // Symbols
    {"`", '`'},
    {"~", '~'},
    {"!", '!'},
    {"@", '@'},
    {"#", '#'},
    {"$", '%'},
    {"^", '^'},
    {"&", '&'},
    {"*", '*'},
    {"(", '('},
    {")", ')'},
    {"-", '-'},
    {"_", '_'},
    {"=", '='},
    {"+", '+'},
    {"\\", '\\'},
    {"|", '|'},
    {";", ';'},
    {":", ':'},
    {"'", '\''},
    {",", ','},
    {"<", '<'},
    {".", '.'},
    {">", '>'},
    {"/", '/'},
    {"?", '?'},
    {"[", '['},
    {"{", '{'},
    {"]", ']'},
    {"}", '}'},
    // Modifiers
    {"LSFT", KEY_LEFT_SHIFT},
    {"LCTL", KEY_LEFT_CTRL},
    {"LALT", KEY_LEFT_ALT},
    {"LGUI", KEY_LEFT_GUI},
    {"RSFT", KEY_RIGHT_SHIFT},
    {"RCTL", KEY_RIGHT_CTRL},
    {"RALT", KEY_RIGHT_ALT},
    {"RGUI", KEY_RIGHT_GUI},
    // System keys
    {"UP", KEY_UP_ARROW},
    {"DOWN", KEY_DOWN_ARROW},
    {"LEFT", KEY_LEFT_ARROW},
    {"RIGHT", KEY_RIGHT_ARROW},
    {"BSPC", KEY_BACKSPACE},
    {"TAB", KEY_TAB},
    {"RETURN", KEY_RETURN},
    {"ENTER", KEY_RETURN}, // Alias for convenience
    {"MENU", KEY_MENU},
    {"ESC", KEY_ESC},
    {"INS", KEY_INSERT},
    {"DEL", KEY_DELETE},
    {"PGUP", KEY_PAGE_UP},
    {"PGDN", KEY_PAGE_DOWN},
    {"HOME", KEY_HOME},
    {"END", KEY_END},
    {"CAPSLK", KEY_CAPS_LOCK},
    {"PRTSC", KEY_PRINT_SCREEN},
    {"SCRLK", KEY_SCROLL_LOCK},
    {"PAUSE", KEY_PAUSE},
    // Function keys
    {"F1", KEY_F1},
    {"F2", KEY_F2},
    {"F3", KEY_F3},
    {"F4", KEY_F4},
    {"F5", KEY_F5},
    {"F6", KEY_F6},
    {"F7", KEY_F7},
    {"F8", KEY_F8},
    {"F9", KEY_F9},
    {"F10", KEY_F10},
    {"F11", KEY_F11},
    {"F12", KEY_F12},
    {"F13", KEY_F13},
    {"F14", KEY_F14},
    {"F15", KEY_F15},
    {"F16", KEY_F16},
    {"F17", KEY_F17},
    {"F18", KEY_F18},
    {"F19", KEY_F19},
    {"F20", KEY_F20},
    {"F21", KEY_F21},
    {"F22", KEY_F22},
    {"F23", KEY_F23},
    {"F24", KEY_F24},
    // Keypad
    {"NUMLK", KEY_NUM_LOCK},
    {"KP0", KEY_KP_0},
    {"KP1", KEY_KP_1},
    {"KP2", KEY_KP_2},
    {"KP3", KEY_KP_3},
    {"KP4", KEY_KP_4},
    {"KP5", KEY_KP_5},
    {"KP6", KEY_KP_6},
    {"KP7", KEY_KP_7},
    {"KP8", KEY_KP_8},
    {"KP9", KEY_KP_9},
    {"KP/", KEY_KP_SLASH},
    {"KP*", KEY_KP_ASTERISK},
    {"KP-", KEY_KP_MINUS},
    {"KP+", KEY_KP_PLUS},
    {"KPENTER", KEY_KP_ENTER},
    {"KP.", KEY_KP_DOT},
};
} // namespace usb
} // namespace hardware