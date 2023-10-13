#pragma once
#include <cstdint>
#include <nlohmann/json.hpp>

struct Config {
  Config() = default;
  Config(const std::string &filename) { load_from_file(filename); };
  Config(int argc, char **argv) { load_from_commandline(argc, argv); };

  void load_from_file(const std::string &filename, std::string program = "");
  void load_from_commandline(int argc, char **argv);
  static void help(std::string program);
  friend std::ostream &operator<<(std::ostream &os, const Config &config);

  std::uint32_t port = 0;
  std::uint8_t address = 0;
};
