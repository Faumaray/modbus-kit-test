#pragma once
#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>

struct Config {
  uint8_t address;
  uint32_t port;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config, address, port)
};
