#pragma once

#include "code.hpp"
#include <cstdint>
#include <variant>
#include <vector>

class Response {
public:
  explicit Response(std::vector<uint8_t> inputData);

  static Response fromRaw(std::vector<uint8_t> inputData) {
    return Response(inputData);
  }
  Response(uint8_t slaveId = 1,
           FunctionCode functionCode = static_cast<FunctionCode>(3),
           uint8_t registersNumber = 1, std::vector<std::uint8_t> values = {})
      : m_header(slaveId, functionCode, registersNumber), m_body(values){};
  Response(uint8_t slaveId = 1,
           ErrorFunctionCode functionCode = static_cast<ErrorFunctionCode>(83),
           ErrorCode errorCode = static_cast<ErrorCode>(1))
      : m_header(slaveId, functionCode, errorCode){};

  Response(uint8_t slaveId = 1,
           FunctionCode functionCode = static_cast<FunctionCode>(3),
           uint8_t registersNumber = 1,
           std::vector<std::uint16_t> values = {});

  Response(const Response &) = default;

  [[nodiscard]] std::vector<uint8_t> toRaw() const;
  friend std::ostream &operator<<(std::ostream &os, const Response &response);

private:
  struct {
    std::uint8_t slaveID;
    std::variant<FunctionCode, ErrorFunctionCode> FunctionCode;
    std::variant<std::uint8_t, ErrorCode> info;
  } m_header;

  std::vector<std::uint8_t> m_body{};
};
