#pragma once

#include "code.hpp"
#include <cstdint>
#include <variant>
#include <vector>

// TODO: split into two sub classes for succes ans error
class Response {
public:
  explicit Response(std::vector<uint8_t> inputData);

  Response(uint8_t slaveId,
           FunctionCode functionCode,
           uint8_t registersNumber, std::vector<std::uint16_t> values)
      : m_header(slaveId, functionCode, registersNumber), m_body(values){};
  Response(uint8_t slaveId,
           ErrorFunctionCode functionCode,
           ErrorCode errorCode)
      : m_header(slaveId, functionCode, errorCode){};

  Response(uint8_t slaveId = 1,
           FunctionCode functionCode = static_cast<FunctionCode>(3),
           uint8_t registersNumber = 1, std::vector<std::uint8_t> values = {});

  Response(const Response &) = default;

  [[nodiscard]] std::vector<uint8_t> toRaw() const;
  friend std::ostream &operator<<(std::ostream &os, const Response &response);

private:
  struct {
    std::uint8_t slaveID;
    std::variant<FunctionCode, ErrorFunctionCode> FunctionCode;
    std::variant<std::uint8_t, ErrorCode> info;
  } m_header;

  std::vector<std::uint16_t> m_body{};
};
