#include "Keyboard.h"

#include "../../../config/config.hpp"
#include "../../hardware/board/DaughterBoard.h"
#include "../../hardware/board/MotherBoard.h"
#include "../../hardware/led/Pixels.h"
#include "../../hardware/txrx/BitBang.h"
#include "../../hardware/usb/Key.h"
#include "../../logic/quantum/Universe.h"
#include "../../utils/Debug.hpp"
#include "Layer.h"

namespace config {
namespace loader {
void Keyboard::Load() {
  hardware::usb::Key::Init();
  LoadHardware();
  LoadLayout();
}

void Keyboard::LoadHardware() {
  DEBUG_INFO("config::loader::Keyboard::LoadHardware");

  const char *jsonString = HARDWARE_CONFIG_JSON;
  DynamicJsonDocument jsonDoc(HARDWARE_CONFIG_JSON_SIZE);
  ParseJson(jsonDoc, jsonString);

  std::string board_uid = GetControllerUid();
  DEBUG_INFO("[INFO] Board UID: %s", board_uid.c_str());
  if (!jsonDoc.containsKey(board_uid)) {
    DEBUG_INFO("[ERROR] No hardware configuration found for %s",
               board_uid.c_str());
    delay(3600000);
  }
  JsonObject config = jsonDoc[board_uid].as<JsonObject>();

  bool isLeft = true;
  bool is_connected = jsonDoc.size() > 1;
  if (is_connected && config.containsKey("data")) {
    isLeft =
        (!config["data"].containsKey("isLeft") || config["data"]["isLeft"]);
    is_connected = true;
    if (config["data"].containsKey("pin")) {

      hardware::txrx::BitBang::Setup((int)config["data"]["pin"], 32, 31250);
    } else {
      DEBUG_INFO("[ERROR] Not connection between boards: "
                 "'.$board_uid.data.pin' not found");
      delay(3600000);
    }
  }
  DEBUG_INFO("[INFO] Board chirality is on the left side and connected: %d, %d",
             isLeft, is_connected);
  randomSeed(isLeft * 42);

  if (config.containsKey("leds")) {
    if (config["leds"].containsKey("count") &&
        config["leds"].containsKey("pin")) {
      hardware::led::Pixels::Setup((int)config["leds"]["pin"],
                                   (int)config["leds"]["count"], isLeft);
    } else {
      DEBUG_INFO("[ERROR] Cannot configure leds: '.{board_uid}.leds.count' or "
                 "'.{board_uid}.leds.pin' not found");
      delay(3600000);
    }
  } else {
    DEBUG_INFO("[INFO] No leds on the board: '.{board_uid}.leds' not found");
  }

  if (config.containsKey("matrix")) {
    if (config["matrix"].containsKey("cols") &&
        config["matrix"].containsKey("rows")) {
      std::vector<unsigned int> col_pins;
      for (JsonVariant item : config["matrix"]["cols"].as<JsonArray>()) {
        col_pins.push_back(item.as<unsigned int>());
      }
      std::vector<unsigned int> row_pins;
      for (JsonVariant item : config["matrix"]["rows"].as<JsonArray>()) {
        row_pins.push_back(item.as<unsigned int>());
      }
      // TODO: dynamically detect which is plugged in
      // * Detect USB Frame?
      // * Detect where the power is coming from (VSYS vs VUSB)?
      if (isLeft) {
        DEBUG_INFO("Connected to USB");
        hardware::board::MotherBoard::Setup(col_pins, row_pins, is_connected);
      } else {
        DEBUG_INFO("Not connected to USB");
        hardware::board::DaughterBoard::Setup(col_pins, row_pins);
      }
    } else {
      DEBUG_INFO(
          "[ERROR] Cannot configure switches: '.$board_uid.matrix.cols' or "
          "'.$board_uid.matrix.rows' not found");
      delay(3600000);
    }
  } else {
    DEBUG_INFO(
        "[WARNING] No switches on the board: '.{board_uid}.matrix' not found");
  }
  DEBUG_INFO("Hardware loaded");
}

void Keyboard::LoadLayout() {
  DEBUG_INFO("config::loader::Keyboard::LoadLayout");

  const char *jsonString = LAYOUT_CONFIG_JSON;
  DynamicJsonDocument jsonDoc(LAYOUT_CONFIG_JSON_SIZE);
  ParseJson(jsonDoc, jsonString);

  std::string default_layer = "";
  for (JsonPair kvp : jsonDoc["layers"].as<JsonObject>()) {
    const std::string layer_name = kvp.key().c_str();
    const JsonObject layer_config = kvp.value().as<JsonObject>();
    config::loader::Layer::Load(layer_name, layer_config);
    if (default_layer == "") {
      default_layer = layer_name;
    }
  }
  DEBUG_INFO("Layout loaded");
  logic::quantum::Universe::Setup(default_layer);
}

std::string Keyboard::GetControllerUid() {
  int len = 2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1;
  char board_uid[len] = "";
  pico_get_unique_board_id_string(board_uid, len);
  return std::string(board_uid);
}

void Keyboard::ParseJson(DynamicJsonDocument &jsonDoc,
                         const char *&jsonString) {
  DEBUG_INFO("%s", jsonString);
  DeserializationError error = deserializeJson(jsonDoc, jsonString);
  if (error) {
    DEBUG_INFO("[ERROR] Parsing failed: %s", error.c_str());
    delay(3600000);
  } else {
    DEBUG_INFO("[INFO] Configuration parsed");
  }
}
} // namespace loader
} // namespace config