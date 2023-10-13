#include "client.hpp"
#include "shared/helper.hpp"
#include "shared/request.hpp"
#include "shared/response.hpp"
#include <cstdint>
#include <iostream>
#include <limits>

template <typename T> T read_user_input(const char *msg) {
  T value{};
  std::cout << msg;
  while (true) {
    std::cin >> value;
    if (!std::cin.fail()) {
      break;
    } else {
      std::cout << "Bad entry. Enter a " << typeid(T).name() << ": ";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  std::cout << std::endl;
  return value;
}

void Client::connect() {
  asio::ip::tcp::resolver resolver(m_context);
  asio::ip::tcp::resolver::query query(m_host, m_port);

  try{
    if(m_socket.is_open()){
        m_socket.close();
    }
    m_socket.connect(*resolver.resolve(query));
    write();
  } catch (std::exception& ec){
    std::cout << "Unable to connect: " << ec.what() << std::endl;

    std::string answer = read_user_input<std::string>("Try again? (y/n): ");
    std::transform(answer.begin(), answer.end(), answer.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    if (!answer.compare("y")) {
      connect();
    } else {
      return;
    }
  }
}

void Client::write(){
    uint16_t address = 1;
    FunctionCode request_type = FunctionCode::ReadAnalogOutputHoldingRegisters;
    uint16_t start = 1;
    uint16_t count = 1;

    address = read_user_input<std::uint16_t>("Enter address: ");
    start = read_user_input<std::uint16_t>("Enter first register: ");
    count = read_user_input<std::uint16_t>("Enter count of registers: ");

    Request request(address, request_type, start, count);

    auto request_raw = request.toRaw();


    m_socket.async_write_some(asio::buffer(request_raw), [this, request, request_raw](std::error_code ec, size_t bytes_written) {
      if (ec) {
        std::cout << "Error: " << ec.message() << std::endl;

        std::string answer = read_user_input<std::string>("Reconnect? (y/n): ");
        std::transform(answer.begin(), answer.end(), answer.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        if (!answer.compare("y")) {
          connect();
        }
      } else {
        std::cout << "Raw request: ";
        print_vector_in_hex(request_raw);
        std::cout << "Stringed request: " << request << "\n";
        std::cout << "data sent: " << request_raw.size() << '/' << bytes_written
                << std::endl;
        read();
      }
    });
}

void Client::read() {
  std::uint8_t buffer[1024];

  m_socket.async_read_some(
      asio::buffer(buffer),
      [this, &buffer](std::error_code ec, size_t bytes_read) {
        if (ec) {
          std::cerr << "Error: " << ec.message() << std::endl;
          
          std::string answer = read_user_input<std::string>("Reconnect? (y/n): ");
          std::transform(answer.begin(), answer.end(), answer.begin(),
                       [](unsigned char c) { return std::tolower(c); });
          if (!answer.compare("y")) {
            connect();
          }
        } else {
          std::vector<std::uint8_t> answer(buffer, buffer + bytes_read - 2);
          Response response(answer);
          std::cout << "Server answer with: ";
          std::cout << "\nRaw: ";
          print_vector_in_hex(response.toRaw());
          std::cout << "Stringed: " << response;
          std::cout << "\nCRC from server: ";
          print_array_in_hex(buffer + bytes_read - 2, 2);
          std::cout << std::endl;
          std::string quit =
                read_user_input<std::string>("Quit? (y/n): ");
            std::transform(quit.begin(), quit.end(), quit.begin(),
                           [](unsigned char c) { return std::tolower(c); });
            if (!quit.compare("y")) {
              return;
            }
            write();
        }
      });
}
