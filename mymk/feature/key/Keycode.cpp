#include "Keycode.h"
#include "../../logic/quantum/Timeline.h"
#include "../../utils/Debug.hpp"

#include "Keyboard.h"
#include <functional>
#include <string>
#include <vector>

std::function<void(Timeline &)>
Keycode::LoadDefinition(const std::string &switch_uid,
                        const std::vector<std::string> &definition) {
  DEBUG_VERBOSE("Keycode::LoadDefinition");
  const std::string keycode = definition[0];
  return [switch_uid, keycode](Timeline &timeline) {
    const std::string timeline_id = "keycode." + keycode;
    Timeline &new_timeline = timeline.split(timeline_id);
    OnPress(new_timeline, switch_uid, keycode);
  };
}

void Keycode::OnPress(Timeline &timeline, const std::string &switch_uid,
                      const std::string &keycode) {
  DEBUG_VERBOSE("Keycode::OnPress");
  std::string press_event = switch_uid + std::string(".pressed");
  std::string release_event = switch_uid + std::string(".released");
  timeline.mark_determined();

  // On commit actions
  DEBUG_INFO("Keycode::OnPress %s: %s", switch_uid.c_str(), keycode.c_str());
  timeline.add_commit_action(
      [keycode](Timeline &timeline) { Keycode::OnCommit(timeline, keycode); });

  // On release configuration
  timeline.add_event_function(
      release_event, [switch_uid, keycode](Timeline &timeline) {
        Keycode::OnRelease(timeline, switch_uid, keycode);
      });
}

void Keycode::OnCommit(Timeline &timeline, const std::string &keycode) {
  DEBUG_INFO("Keycode pressed: %s", keycode.c_str());
  if (keycode != "NONE") {
    Keyboard.press(keycodes[keycode]);
  }
}

void Keycode::OnRelease(Timeline &timeline, const std::string &switch_uid,
                        const std::string &keycode) {
  DEBUG_VERBOSE("Keycode::OnRelease");
  timeline.add_commit_action([keycode](Timeline &) {
    DEBUG_INFO("Keycode released: %s", keycode.c_str());
    if (keycode != "NONE") {
      Keyboard.release(keycodes[keycode]);
    }
  });
  std::string release_event = switch_uid + std::string(".released");
  timeline.remove_event_function(release_event);
}

std::map<std::string, char> Keycode::keycodes = {
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