#ifndef MYMK_CONFIG_LOADER
#define MYMK_CONFIG_LOADER

#include "../feature/Layer.hpp"
#include "../hardware/BitBang.hpp"
#include "../hardware/board/DaughterBoard.hpp"
#include "../hardware/board/MotherBoard.hpp"
#include "../hardware/led/Pixels.hpp"

#include <ArduinoJson.h>
#include <string>
#include <vector>

#include "../../config/config.hpp"

void parse_json(DynamicJsonDocument &jsonDoc, const char *&jsonString) {
  DEBUG_INFO("%s", jsonString);
  DeserializationError error = deserializeJson(jsonDoc, jsonString);
  if (error) {
    DEBUG_INFO("[ERROR] Parsing failed: %s", error.c_str());
    delay(3600000);
  } else {
    DEBUG_INFO("[INFO] Configuration parsed");
  }
}

std::string get_controller_uid() {
  int len = 2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1;
  char board_uid[len] = "";
  pico_get_unique_board_id_string(board_uid, len);
  return std::string(board_uid);
}

void load_board() {
  DEBUG_INFO("Loading board");

  const char *jsonString = BOARD_CONFIG_JSON;
  DynamicJsonDocument jsonDoc(BOARD_CONFIG_JSON_SIZE);
  parse_json(jsonDoc, jsonString);

  std::string board_uid = get_controller_uid();
  DEBUG_INFO("[INFO] Board UID: %s", board_uid.c_str());
  if (!jsonDoc.containsKey(board_uid)) {
    DEBUG_INFO("[ERROR] No configuration found for %s", board_uid.c_str());
    delay(3600000);
  }

  if (!jsonDoc.containsKey(board_uid)) {
    DEBUG_INFO("[ERROR] No configuration found for %s", board_uid.c_str());
    delay(3600000);
  }
  JsonObject config = jsonDoc[board_uid].as<JsonObject>();

  bool isLeft = true;
  bool is_connected = false;
  if (config.containsKey("data")) {
    isLeft =
        (!config["data"].containsKey("isLeft") || config["data"]["isLeft"]);
    is_connected = true;
    if (config["data"].containsKey("pin")) {

      BitBang::initialize((int)config["data"]["pin"], 31250);
    } else {
      DEBUG_INFO("[ERROR] Not connection between boards: "
                 "'.{board_uid}.data.pin' not found");
      delay(3600000);
    }
  }
  DEBUG_INFO("[INFO] Board chirality is on the left side: %d", isLeft);
  randomSeed(isLeft * 42);

  if (config.containsKey("leds")) {
    if (config["leds"].containsKey("count") &&
        config["leds"].containsKey("pin")) {
      set_pixels((int)config["leds"]["pin"], (int)config["leds"]["count"],
                 isLeft);
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
      const unsigned int msgLength = 32;
      if (isLeft) {
        MotherBoard::Setup(msgLength, col_pins, row_pins, is_connected);
      } else {
        DaughterBoard::Setup(msgLength, col_pins, row_pins);
      }
    } else {
      DEBUG_INFO(
          "[ERROR] Cannot configure switches: '.{board_uid}.matrix.cols' or "
          "'.{board_uid}.matrix.rows' not found");
      delay(3600000);
    }
  } else {
    DEBUG_INFO(
        "[WARNING] No switches on the board: '.{board_uid}.matrix' not found");
  }
  DEBUG_INFO("Board loaded");
};

void load_layout() {
  DEBUG_INFO("Loading layout");

  const char *jsonString = LAYOUT_CONFIG_JSON;
  DynamicJsonDocument jsonDoc(LAYOUT_CONFIG_JSON_SIZE);
  parse_json(jsonDoc, jsonString);

  for (JsonPair kvp : jsonDoc["layers"].as<JsonObject>()) {
    const std::string layer_name = kvp.key().c_str();
    const JsonObject layer_config = kvp.value().as<JsonObject>();
    Layer::Load(layer_name, layer_config);
  }
  DEBUG_INFO("Layout loaded");
};

void load_config() {
  load_board();
  load_layout();
};
#endif