#include "session.hpp"
#include <asio/write.hpp>
#include <iostream>
#include "shared/crc.hpp"
#include "shared/helper.hpp"

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

    std::vector<uint8_t> data = {m_data[0]};
  
    if(m_data[0] != m_device->getAddress()){
        
        data.emplace_back(m_data[1]+128);
        data.emplace_back(2);
  
    } else {
        uint16_t start = convert_8_to_16(m_data[2], m_data[3]);
        uint16_t count = convert_8_to_16(m_data[4], m_data[5]);
        
        auto response = m_device->read(start, count);


        if(response[0] != (m_data[1]+128)){

            data.emplace_back(m_data[1]);
            data.emplace_back((uint8_t)(count*2));
                
            for(auto value : response){
                auto split = split_16_to_Hi_and_Lo(value);
                data.emplace_back(split[0]);
                data.emplace_back(split[1]);
            }
        }else {
            data.emplace_back(data[1]+128);
            data.emplace_back((uint8_t)response[1]);
        }
    }
    auto crc = crc16(data.data(), data.size());
    std::array<uint8_t,2> crc_split = split_16_to_Lo_and_Hi(crc);
    data.emplace_back(crc_split[0]);
    data.emplace_back(crc_split[1]);
    data.shrink_to_fit();

  asio::async_write(m_socket, asio::buffer(data, data.size()),
                    [this, self, data](std::error_code ec, std::size_t /*length*/) {
                      if (!ec) {
                        std::cout << "Server reply: \n";
                        print_vector_in_hex(data);
                        schedule_read();
                      }
                    });
}


