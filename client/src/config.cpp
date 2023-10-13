#include "config.hpp"
#include <fstream>
#include <iostream>

void Config::load_from_file(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Could not open configuration file: " << filename << std::endl;
    exit(1);
  }

  nlohmann::json json_config;
  file >> json_config;
  address = json_config["address"].get<uint8_t>();
  port = json_config["port"].get<uint32_t>();
}

void Config::load_from_commandline(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "--address") {
      address = argv[i + 1];
      i++;
    } else if (arg == "--port") {
      port = std::stoi(argv[i + 1]);
      i++;
    } else if (arg == "--help") {
      help(argv[0]);
      exit(0);
    }
  }
}

void Config::help(char *program) {
  std::cout << "Usage: " << program
            << " [--address <address>] [--port <port>] [--config "
               "<config_file>]\n\n"
            << "address(string): The address of the server to connect to. This "
               "can be "
               "either an IP address or a hostname.\n"
            << "port(number): The port number to connect to on the server.\n\n"
            << "Default config file name: config_client.json\n\n"
            << "Config file format:\n"
               "{\n\t\"address\": \"127.0.0.1\",\n\t\"port\": 8080\n}"
            << std::endl;
}
std::ostream &operator<<(std::ostream &os, const Config &config) {

  os << "Launching client with configuration:"
     << "\n  address: " << config.address << "\n  port: " << config.port;

  return os;
}
