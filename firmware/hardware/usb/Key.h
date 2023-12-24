#ifndef MYMK_HARDWARE_USB_USBKEY
#define MYMK_HARDWARE_USB_USBKEY

#include <map>
#include <memory>
#include <string>

namespace hardware {
namespace usb {
class Key {
public:
  typedef std::shared_ptr<Key> Ptr;

private:
  static std::map<const std::string, const char> definition2code;
  static std::map<const char, Ptr> map;
  const char keycode;
  unsigned int count;

public:
  Key(const char keycode);

  static Ptr New(const char keycode);

  static void Init();

  static void Press(const std::string key_definition);

  static void Release(const std::string key_definition);
};
} // namespace usb
} // namespace hardware
#endif