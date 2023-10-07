#include <asio.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include "shared/crc.hpp"
#include "shared/helper.hpp"
#include "config.hpp"

auto main(int argc, char *argv[]) -> int {
  try {
    asio::io_context io_context;

    std::ifstream f("config_client.json");
    nlohmann::json json = nlohmann::json::parse(f);
    auto config = json.template get<Config>();
    // we need a socket and a resolver
    asio::ip::tcp::socket socket(io_context);
    asio::ip::tcp::resolver resolver(io_context);

    // now we can use connect(..)
    asio::connect(socket, resolver.resolve(config.address, config.port));

        uint16_t address = 1;
        uint8_t request_type = 3;
        uint16_t start = 9;
        uint16_t len = 3;

        char quit;
    do{
        std::cout << "Enter address: ";
        std::cin >> address;
        std::cout << "Enter first register: ";
        std::cin >> start;
        auto start_split = split_16_to_Hi_and_Lo(start);
        std::cout << "Enter count of registers: ";
        std::cin >> len;
        auto len_split = split_16_to_Hi_and_Lo(len);
        
        std::vector<uint8_t> request{
            (uint8_t)address, request_type,
            start_split[0], start_split[1],
            len_split[0], len_split[1]
        };

        auto crc = crc16(request.data(), request.size());
        auto crc_split = split_16_to_Lo_and_Hi(crc) ;
        request.emplace_back(crc_split[0]);
        request.emplace_back(crc_split[1]);

        request.shrink_to_fit();

        auto result = asio::write(socket, asio::buffer(request, request.size()));

        std::cout << "data sent: " << request.size() << '/' << result << std::endl;

        std::array<uint8_t,3> header;
        size_t reply_length = asio::read(socket, asio::buffer(header, 3));
        if(header[1] == request_type){
            uint8_t body[header[2]];
            size_t body_length = asio::read(socket, asio::buffer(body, header[2]));
            std::cout << "Server answer: \n";

            std::cout << "Head: ";
            print_array_in_hex(header.data(), header.size());
            std::cout << "Body: ";
            print_array_in_hex(body, body_length);

        }else{
            std::cout << "Server answer with error: ";

            print_array_in_hex(header.data(), header.size());
        }

            std::array<uint8_t,2> crcReply;
        size_t crc_length = asio::read(socket, asio::buffer(crcReply, header[2]));
        std::cout << "CRC: ";

        print_array_in_hex(crcReply.data(), crcReply.size());

        std::cout << "Enter any key to continue or q for exit" << std::endl;
        std::cin >> quit;
    }while(quit!='q');

    asio::error_code ec;
    socket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
    socket.close();
  } catch (nlohmann::json::exception &je){
    std::cerr << "Exception: " << je.what() << "\n"
            << "Config in file config_client.json\n"
            << "Config Example: \n"
            << "{\n\"address\": \"127.0.0.1\",\n\"port\": \"8080\"\n}" << std::endl;
  } catch (std::exception e) {
    std::cerr << "Error occured: " << e.what() << std::endl;
  }

  return 0;
}
