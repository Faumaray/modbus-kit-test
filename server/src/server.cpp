#include "server.hpp"
#include "session.hpp"
#include <iostream>

void Server::schedule_accept() {
  std::cout<< std::dec << "Listening on http://localhost:"
            << m_acceptor.local_endpoint().port() << std::endl;
  m_acceptor.async_accept(
      [this](std::error_code ec, asio::ip::tcp::socket socket) {
        if (!ec) {
          std::make_shared<Session>(std::move(socket), m_device)->start();
        }
        schedule_accept();
      });
}
