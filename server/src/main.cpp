#include <asio.hpp>
#include "config.hpp"
#include "server.hpp"
#include <iostream>
#include "device.hpp"
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>


auto main(int argc, char *argv[]) -> int {
  try {
    Config config;
    config.load_from_commandline(argc, argv);

      if (argc > 1 && argv[1] == "--config") {
        config.load_from_file(argv[2]);
      }
      if (config.address == 0 || config.port == 0){
          config.load_from_file("config_server.json");
      }

    std::cout << config << std::endl;
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
    std::cerr << "Exception: " << je.what() 
            << std::endl;
    Config::help(argv[0]);
    return 1;
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    Config::help(argv[0]);
    return 1;
  }

  return 0;
}
