#pragma once
#include <array>
#include <cstdint>
#include <vector>

class Device {
public:
  Device(uint16_t address) : m_address(address){};
  ~Device() {}

  std::vector<uint16_t> read(int start, int count) const;
  uint8_t getAddress() const { return m_address; };

private:
  uint8_t m_address;
  std::array<uint16_t, 10> m_holding_regs = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
};
