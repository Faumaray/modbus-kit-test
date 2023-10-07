#include "helper.hpp"

void print_in_hex(uint8_t value) {
  std::cout << "[" << std::hex << std::setfill('0') << std::setw(2)
            << static_cast<int>(value) << "] ";
}

void print_vector_in_hex(std::vector<uint8_t> vec) {
  for (const auto &value : vec) {
    print_in_hex(value);
  }
  std::cout << std::endl;
}

void print_array_in_hex(uint8_t *vec, size_t size) {
  for (auto i = 0; i < size; i++) {
    print_in_hex(vec[i]);
  }
  std::cout << std::endl;
}

uint16_t convert_8_to_16(uint8_t first, uint8_t second) {
  return ((uint16_t)first << 8 | second);
}

std::array<uint8_t, 2> split_16_to_Hi_and_Lo(uint16_t value) {
  return {(uint8_t)(value >> 8), (uint8_t)(value & 0xFF)};
}

std::array<uint8_t, 2> split_16_to_Lo_and_Hi(uint16_t value) {
  return {(uint8_t)(value & 0x00FF), (uint8_t)(value >> 8)};
}
