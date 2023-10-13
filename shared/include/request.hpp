#pragma once

#include "code.hpp"
#include <cstdint>
#include <vector>

// TODO: extend to use coils and write operation
class Request {
public:
  explicit Request(const std::vector<uint8_t> &inputData);
  explicit Request(uint8_t slaveId = 1,
                   FunctionCode functionCode = static_cast<FunctionCode>(3),
                   uint16_t address = 1, uint16_t registersNumber = 1) noexcept
      : m_header(slaveId, functionCode), m_body(address, registersNumber){};

  Request(const Request &) = default;

  static Request fromRaw(const std::vector<uint8_t> &inputData) {
    return Request(inputData);
  }
  [[nodiscard]] std::vector<uint8_t> toRaw() const noexcept;
  [[nodiscard]] uint8_t slaveID() const { return m_header.slaveID; }
  [[nodiscard]] FunctionCode functionRegisters() const noexcept {
    return m_header.code;
  }
  [[nodiscard]] uint16_t registerAddress() const { return m_body.start; }
  [[nodiscard]] uint16_t numberOfRegisters() const { return m_body.count; }

  void setSlaveId(uint8_t slaveId) { m_header.slaveID = slaveId; }
  void setFunctionCode(FunctionCode functionCode) {
    m_header.code = functionCode;
  }
  void setAddress(uint16_t address) { m_body.start = address; }
  void setCount(uint16_t count) { m_body.count = count; }

  friend std::ostream &operator<<(std::ostream &os, const Request &request);

private:
  struct {
    std::uint8_t slaveID;
    FunctionCode code;
  } m_header;
  struct body {
    std::uint16_t start;
    std::uint16_t count;
  } m_body;
};
