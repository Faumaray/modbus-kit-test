#pragma once
#include <cstdint>
#include <nlohmann/json.hpp>

struct Config {
  Config() = default;
  Config(const std::string& filename) {load_from_file(filename);};
  Config(int argc, char** argv) {load_from_commandline(argc, argv);};


  void load_from_file(const std::string& filename);
  void load_from_commandline(int argc, char  **argv);
  static void help(char* program);
  friend std::ostream &operator<<(std::ostream &os, const Config &config);

  uint8_t address;
  uint32_t port;
};
