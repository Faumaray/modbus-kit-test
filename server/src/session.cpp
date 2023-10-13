#include "session.hpp"
#include <asio/write.hpp>
#include <iostream>
#include "shared/helper.hpp"
#include "shared/response.hpp"

void Session::start() { schedule_read(); }

void Session::schedule_read() {
  auto self(shared_from_this());

  m_socket.async_read_some(
      asio::buffer(m_data, max_length),
      [this, self](std::error_code ec, std::size_t length) {
        if (!ec) {
                std::cout << "Client sent: \n";
                print_array_in_hex(m_data.data(), m_data.size());
                schedule_write(length);
        }
      });
}

void Session::schedule_write(std::size_t length) {
    auto self(shared_from_this());

    Response response(1, ErrorFunctionCode::Undefined, ErrorCode::SlaveDeviceFailure);
    if(m_data[0] != m_device->getAddress()){
        std::cout << "Wrong slave";
        response = Response(1, ErrorFunctionCode::ReadAnalogOutputHoldingRegisters, ErrorCode::IllegalDataAddress);
        std::cout << response << std::endl;
    } else {
        uint16_t start = convert_8_to_16(m_data[2], m_data[3]);
        uint16_t count = convert_8_to_16(m_data[4], m_data[5]);
        
        auto device_response = m_device->read(start, count);


        if(device_response[0] != static_cast<std::uint8_t>(ErrorFunctionCode::ReadAnalogOutputHoldingRegisters)){
            response = Response(1, FunctionCode::ReadAnalogOutputHoldingRegisters, count*2, device_response);
        }else {
            response = Response(1, ErrorFunctionCode::ReadAnalogOutputHoldingRegisters, ErrorCode::IllegalDataAddress);
        }
    }
    auto response_raw = response.toRaw();

  asio::async_write(m_socket, asio::buffer(response_raw),
                    [this, self, response, response_raw](std::error_code ec, std::size_t /*length*/) {
                      if (!ec) {
                        std::cout << "Server reply: \nRaw: ";
                        print_vector_in_hex(response_raw);
                        std::cout << "Stringed: " << response << std::endl;
                        schedule_read();
                      }
                    });
}


