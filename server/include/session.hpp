#pragma once
#include "device.hpp"
#include <asio/ip/tcp.hpp>

class Session : public std::enable_shared_from_this<Session> {
public:
  explicit Session(asio::ip::tcp::socket socket, std::shared_ptr<Device> device)
      : m_socket(std::move(socket)), m_device(device){};

  void start();

private:
  void schedule_read();
  void schedule_write(size_t length);

private:
  asio::ip::tcp::socket m_socket;
  std::shared_ptr<Device> m_device;
  static constexpr size_t max_length = 8;
    std::array<uint8_t, max_length> m_data{};
};
