#ifndef MYMK_CONFIG_LOADER
#define MYMK_CONFIG_LOADER
#include "../hardware/BitBang.hpp"
#include "../hardware/DaughterBoard.hpp"
#include "../hardware/MotherBoard.hpp"
#include "../hardware/Pixels.hpp"
#include <ArduinoJson.h>
#include <vector>

#include "../../config/config.hpp"

std::string get_controller_uid() {
  int len = 2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1;
  char board_uid[len] = "";
  pico_get_unique_board_id_string(board_uid, len);
  return std::string(board_uid);
}

void load_board() {
  Serial.println("Loading board");

  const char *jsonString = BOARD_CONFIG_JSON;
  Serial.println(jsonString);

  // Decode the document
  DynamicJsonDocument jsonDoc(1000);
  DeserializationError error = deserializeJson(jsonDoc, jsonString);

  if (error) {
    Serial.print("[ERROR] Parsing failed: ");
    Serial.println(error.c_str());
    delay(3600000);
  } else {
    Serial.println("[INFO] Board configuration parsed");
  }

  std::string board_uid = get_controller_uid();
  Serial.print("[INFO] Board UID: ");
  Serial.println(board_uid.c_str());

  if (!jsonDoc.containsKey(board_uid)) {
    Serial.print("[ERROR] No configuration found for ");
    Serial.println(board_uid.c_str());
    delay(3600000);
  }
  JsonObject config = jsonDoc[board_uid].as<JsonObject>();

  bool isLeft = true;
  if (config.containsKey("data")) {
    isLeft =
        (!config["data"].containsKey("isLeft") || config["data"]["isLeft"]);
    if (config["data"].containsKey("pin")) {
      BitBang::initialize((int)config["data"]["pin"], 31250);
    } else {
      Serial.println("[ERROR] Not connection between boards: "
                     "'.{board_uid}.data.pin' not found");
      delay(3600000);
    }
  }
  Serial.print("[INFO] Board chirality is on the left side: ");
  Serial.println(isLeft);

  if (config.containsKey("leds")) {
    if (config["leds"].containsKey("count") &&
        config["leds"].containsKey("pin")) {
      set_pixels((int)config["leds"]["pin"], (int)config["leds"]["count"],
                 isLeft);
    } else {
      Serial.println(
          "[ERROR] CAnnot configure leds: '.{board_uid}.leds.count' or "
          "'.{board_uid}.leds.pin' not found");
      delay(3600000);
    }
  } else {
    Serial.println(
        "[INFO] No leds on the board: '.{board_uid}.leds' not found");
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
        MotherBoard::Setup(msgLength, col_pins, row_pins);
      } else {
        DaughterBoard::Setup(msgLength, col_pins, row_pins);
      }
    } else {
      Serial.println(
          "[ERROR] CAnnot configure leds: '.{board_uid}.leds.count' or "
          "'.{board_uid}.leds.pin' not found");
      delay(3600000);
    }
  } else {
    Serial.println(
        "[WARNING] No switches on the board: '.{board_uid}.matrix' not found");
  }
  Serial.println("Board loaded");
};

void load_layout() {
  Serial.println("Loading layout");
  Serial.println("Layout loaded");
};

void load_config() {
  load_board();
  load_layout();
};
#endif