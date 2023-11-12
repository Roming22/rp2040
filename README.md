# MYMK
Custom keyboard firmware generator, in the vein of QMK and KMK.

## Features
* Full configuration through YAML files
* Split keyboard support

## Differences with QMK
* Support for input backtracking (i.e. when the output cannot be decided until additional key presses, or the lack of thereof, is detected)
* Per layer combos

## Required Arduino dependencies
Boards managers:
* [Raspberry Pi Pico/RP2040](https://github.com/earlephilhower/arduino-pico#installing-via-arduino-boards-manager))

Libraries:
* [Adafruit NeoPixels](https://adafruit.github.io/Adafruit_NeoPixel/html/index.html)
* [Arduino_DebugUtils](https://github.com/arduino-libraries/Arduino_DebugUtils#readme)
* [ArduinoJson](https://arduinojson.org/v6/doc/)
* [Keyboard](https://www.arduino.cc/reference/en/language/functions/usb/keyboard/)

## Other dependencies
* [jq](https://github.com/jqlang/jq#readme)
* [yq](https://github.com/mikefarah/yq#readme)

## Controller support
* [Sea-Picro](https://github.com/joshajohnson/sea-picro/blob/master/README.md)

## Technical References
* [Arduino reference](https://www.arduino.cc/reference/en/)
* [Raspberry Pi Pico SDK](https://www.raspberrypi.com/documentation/pico-sdk/hardware.html)
* [Sea-Picro pinout](https://github.com/joshajohnson/sea-picro/blob/master/README.md#pinout)
* [Hillside 46](https://github.com/mmccoyd/hillside/tree/main/hillside46)
