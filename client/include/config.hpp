#pragma once
#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>

struct Config {
  std::string address;
    std::string port;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config, address, port)
};
