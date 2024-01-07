#include "Keyboard.h"

#include "../../../config/config.hpp"
#include "../../hardware/board/DaughterBoard.h"
#include "../../hardware/board/MotherBoard.h"
#include "../../hardware/led/Pixels.h"
#include "../../hardware/txrx/BitBang.hpp"
#include "../../hardware/usb/Key.h"
#include "../../logic/quantum/Universe.h"
#include "../../utils/Debug.hpp"
#include "Layer.h"

namespace config {
namespace loader {
void Keyboard::Load() {
  hardware::usb::Key::Init();
  LoadHardware();
  if (hardware::board::BaseBoard::IsMotherboard()) {
    LoadLayout();
  }
}

void Keyboard::LoadHardware() {
  DEBUG_INFO("config::loader::Keyboard::LoadHardware");

  const char *jsonString = HARDWARE_CONFIG_JSON;
  JsonDocument jsonDoc;
  ParseJson(jsonDoc, jsonString);

  std::string board_uid = GetControllerUid();
  DEBUG_INFO("[INFO] Board UID: %s", board_uid.c_str());
  if (!jsonDoc.containsKey(board_uid)) {
    DEBUG_INFO("[ERROR] No hardware configuration found for %s",
               board_uid.c_str());
    delay(3600000);
  }
  JsonObject config = jsonDoc[board_uid].as<JsonObject>();

  bool isLeft =
      (!config["data"].containsKey("isLeft") || config["data"]["isLeft"]);

  // LEDS
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
    hardware::led::Pixels::Set(4, 255 * isLeft, 255 * (1 - isLeft), 0);
  } else {
    DEBUG_INFO("[INFO] No leds on the board: '.{board_uid}.leds' not found");
  }

  // TXRX
  bool is_connected = config.containsKey("data");
  if (is_connected) {
    if (config["data"].containsKey("pin")) {
      hardware::txrx::BitBang::Setup((int)config["data"]["pin"], 32, 125000);
    } else {
      DEBUG_INFO("[ERROR] Not connection between boards: "
                 "'.%s.data.pin' not found",
                 board_uid.c_str());
      delay(3600000);
    }
  }

  // BOARD
  // TODO: dynamically detect which is plugged in
  // * Detect USB Frame?
  // * Detect where the power is coming from (VSYS vs VUSB)?
  bool isMotherBoard = isLeft;
  DEBUG_INFO("[INFO] %s is on the %s side and %s connected",
             isMotherBoard ? "MotherBoard" : "DaughterBoard",
             isLeft ? "left" : "right", is_connected ? "is" : "is not");
  randomSeed(isMotherBoard * 42);
  if (config.containsKey("matrix")) {
    std::vector<unsigned int> col_pins;
    std::vector<unsigned int> row_pins;
    if (config["matrix"].containsKey("cols")) {
      for (JsonVariant item : config["matrix"]["cols"].as<JsonArray>()) {
        col_pins.push_back(item.as<unsigned int>());
      }
    } else {
      DEBUG_INFO(
          "[ERROR] Cannot configure switches: '.%s.matrix.cols' not found",
          board_uid.c_str());
      delay(3600000);
    }
    if (config["matrix"].containsKey("rows")) {
      for (JsonVariant item : config["matrix"]["rows"].as<JsonArray>()) {
        row_pins.push_back(item.as<unsigned int>());
      }
    } else {
      DEBUG_INFO(
          "[ERROR] Cannot configure switches: '.%s.matrix.rows' not found",
          board_uid.c_str());
      delay(3600000);
    }
    if (isMotherBoard) {
      hardware::board::MotherBoard::Setup(col_pins, row_pins, is_connected);
    } else {
      hardware::board::DaughterBoard::Setup(col_pins, row_pins);
    }
  } else {
    DEBUG_INFO("[WARNING] No switches on the board: '.%s.matrix' not found",
               board_uid.c_str());
  }
  DEBUG_INFO("");
}

void Keyboard::LoadLayout() {
  DEBUG_INFO("config::loader::Keyboard::LoadLayout");

  const char *jsonString = LAYOUT_CONFIG_JSON;
  JsonDocument jsonDoc;
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

void Keyboard::ParseJson(JsonDocument &jsonDoc, const char *&jsonString) {
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