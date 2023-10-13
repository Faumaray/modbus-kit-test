#include "client.hpp"
#include "config.hpp"
#include <asio.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>


auto main(int argc, char *argv[]) -> int {
  try {
    asio::io_context io_context;
    Config config;
    config.load_from_commandline(argc, argv);

      if (argc > 1 && argv[1] == "--config") {
        config.load_from_file(argv[2]);
      }
      if (config.address.empty() || config.port == 0){
          config.load_from_file("config_client.json");
      }
    std::cout << config << std::endl;

      Client client(io_context, config.address, std::to_string(config.port));

      client.connect();

      io_context.run();

  } catch (nlohmann::json::exception &je) {
    std::cerr << "Exception: " << je.what()
              << std::endl;
    Config::help(argv[0]);
    return 1;
  } catch (std::exception e) {
    std::cerr << "Error occured: " << e.what() << std::endl;
    Config::help(argv[0]);
    return  1;
  }

  return 0;
}
