#pragma once

#include "device.hpp"
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>

class Server {
public:
  Server(asio::io_context &context, short port, std::shared_ptr<Device> device)
      : m_acceptor(context,
                   asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), m_device(std::move(device)) {
    schedule_accept();
  };

private:
  void schedule_accept();

private:
  asio::ip::tcp::acceptor m_acceptor;
  std::shared_ptr<Device> m_device;
};
