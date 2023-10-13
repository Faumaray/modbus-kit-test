#pragma once

#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
class Client {
public:
  Client(asio::io_context &io_context, const std::string &host, const std::string &port) : m_context(io_context), m_socket(io_context), m_host(host), m_port(port){};
  ~Client(){
        m_socket.close();
  }

  void connect();

private:
  void read();
  void write();
  std::string m_host;
  std::string m_port;
  asio::io_context &m_context;
  asio::ip::tcp::socket m_socket;
};
