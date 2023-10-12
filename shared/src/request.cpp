#include "request.hpp"
#include "code.hpp"
#include "crc.hpp"
#include "helper.hpp"

Request::Request(const std::vector<uint8_t> &inputData) {
    m_header.slaveID = inputData[0];
    m_header.code = FunctionCode(inputData[1]);
    m_body.start = convert_8_to_16(inputData[2], inputData[3]);
    m_body.count = convert_8_to_16(inputData[4], inputData[5]);
}

std::ostream &operator<<(std::ostream &os, const Request &request){
    os << request.m_header.code << ", from slave "
        << request.m_header.slaveID
        << ", starting from address " << request.m_body.start
        << ", on " << request.m_body.count << " registers";
    
    return os;
}

std::vector<uint8_t> Request::toRaw() const noexcept {
  std::vector<uint8_t> result;
  result.reserve(8);

  result.emplace_back(m_header.slaveID);
  result.emplace_back(static_cast<std::uint8_t>(m_header.code));


  auto start_split = split_16_to_Hi_and_Lo(m_body.start);
  auto count_split = split_16_to_Hi_and_Lo(m_body.count);


  result.emplace_back(start_split[0]);
  result.emplace_back(start_split[1]);

  result.emplace_back(count_split[0]);
  result.emplace_back(count_split[1]);

  auto crc = crc16(result.data(), result.size());
  auto crc_split = split_16_to_Lo_and_Hi(crc);
  result.emplace_back(crc_split[0]);
  result.emplace_back(crc_split[1]);
 
  return result;
}
