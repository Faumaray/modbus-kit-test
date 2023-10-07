#include "device.hpp"

std::vector<uint16_t> Device::read(int start, int count) const {
  if ((start - 1) > 10 || (start + count - 1) > 10 || start < 1 || count < 1) {
    return std::vector<uint16_t>{131, 2};
  }
  auto regs_start = m_holding_regs.cbegin() + start - 1;
  return std::vector<uint16_t>(regs_start, regs_start + count);
}
