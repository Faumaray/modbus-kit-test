#include <asio.hpp>
#include "config.hpp"
#include "server.hpp"
#include <iostream>
#include <fstream>
#include "device.hpp"
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>


auto main(int argc, char *argv[]) -> int {
  try {
    std::ifstream f("config_server.json");
    nlohmann::json json = nlohmann::json::parse(f);
    auto config = json.template get<Config>();
    asio::io_context io_context;
    auto device = std::make_shared<Device>(config.address);
    Server s(io_context, config.port, device);
    asio::thread_pool pool;
    for (unsigned i = 0; i < std::thread::hardware_concurrency(); ++i) {
      asio::post(pool, [&io_context] {
        io_context.run();
      });
    }

  } catch (nlohmann::json::exception &je){
    std::cerr << "Exception: " << je.what() << "\n"
            << "Config in file config_server.json\n"
            << "Config Example: \n"
            << "{\n\"address\": 1,\n\"port\": 8080\n}" << std::endl;
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  return 0;
}
