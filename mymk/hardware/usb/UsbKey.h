#ifndef MYMK_HARDWARE_USB_USBKEY
#define MYMK_HARDWARE_USB_USBKEY

#include <map>
#include <string>

class UsbKey {
private:
  static std::map<const std::string, const char> definition2code;
  static std::map<const char, UsbKey *> map;
  const char keycode;
  unsigned int count;

public:
  UsbKey(const char keycode);

  static void Init();

  static void Press(const std::string key_definition);

  static void Release(const std::string key_definition);
};
#endif