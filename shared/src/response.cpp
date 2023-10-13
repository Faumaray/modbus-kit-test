#include "response.hpp"
#include "code.hpp"
#include "crc.hpp"
#include "helper.hpp"
#include <cstdint>
#include <iostream>
#include <variant>

Response::Response(uint8_t slaveId, FunctionCode functionCode,
                   uint8_t registersNumber, std::vector<std::uint8_t> values)
    : m_header(slaveId, functionCode, registersNumber) {
  m_body.reserve(registersNumber / 2);
  for (auto i = 0; i < values.size() - 1; i += 2) {
    auto combine = convert_8_to_16(values[i], values[i + 1]);
    m_body.emplace_back(combine);
  }
}

Response::Response(std::vector<uint8_t> inputData) {
  m_header.slaveID = inputData[0];
  if (inputData[1] >= 0x80) {
    m_header.FunctionCode = static_cast<ErrorFunctionCode>(inputData[1]);
    m_header.info = static_cast<ErrorCode>(inputData[2]);
  } else {
    m_header.FunctionCode = static_cast<FunctionCode>(inputData[1]);
    m_header.info = inputData[2];
    m_body.reserve(inputData[2]);
    for (auto i = 3; i < inputData.size() - 1; i += 2) {
      auto combine = convert_8_to_16(inputData[i], inputData[i + 1]);
      m_body.emplace_back(combine);
    }
  }
}

std::ostream &operator<<(std::ostream &os, const Response &response) {
  if (std::holds_alternative<FunctionCode>(response.m_header.FunctionCode)) {
    os << std::get<FunctionCode>(response.m_header.FunctionCode)
       << ", from slave " << std::to_string(response.m_header.slaveID);
    os << ", returned next "
       << std::to_string(std::get<std::uint8_t>(response.m_header.info) / 2)
       << " values: ";
    for (const auto &value : response.m_body) {
      os << "[" << std::to_string(value) << "] ";
    }
  } else {
    std::string res;
    os << "Error on slave " << std::to_string(response.m_header.slaveID);
    os << " - " << std::get<ErrorCode>(response.m_header.info);
    os << " ( on function: "
       << std::get<ErrorFunctionCode>(response.m_header.FunctionCode) << " )";
  }
  return os;
}

std::vector<uint8_t> Response::toRaw() const {
  std::vector<uint8_t> result;
  result.reserve(5);

  result.emplace_back(m_header.slaveID);
  if (std::holds_alternative<FunctionCode>(m_header.FunctionCode)) {
    std::uint8_t functionCode = static_cast<std::uint8_t>(
        std::get<FunctionCode>(m_header.FunctionCode));
    std::uint8_t count =
        static_cast<std::uint8_t>(std::get<std::uint8_t>(m_header.info));
    result.emplace_back(functionCode);
    result.emplace_back(count);
    result.reserve(m_body.size());
    for (auto value : m_body) {
      auto split = split_16_to_Hi_and_Lo(value);
      result.emplace_back(split[0]);
      result.emplace_back(split[1]);
    }
  } else {
    std::uint8_t functionCode = static_cast<std::uint8_t>(
        std::get<ErrorFunctionCode>(m_header.FunctionCode));
    std::uint8_t count =
        static_cast<std::uint8_t>(std::get<ErrorCode>(m_header.info));
    result.emplace_back(functionCode);
    result.emplace_back(count);
  }

  auto crc = crc16(result.data(), result.size());
  std::array<uint8_t, 2> crc_split = split_16_to_Lo_and_Hi(crc);
  result.emplace_back(crc_split[0]);
  result.emplace_back(crc_split[1]);
  result.shrink_to_fit();

  return result;
}
