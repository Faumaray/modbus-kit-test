#pragma once

#include <array>
#include <cstdint>
#include <vector>

void print_in_hex(uint8_t value);
void print_vector_in_hex(std::vector<uint8_t> vec);

void print_array_in_hex(uint8_t *vec, size_t size);

uint16_t convert_8_to_16(uint8_t first, uint8_t second);

std::array<uint8_t, 2> split_16_to_Hi_and_Lo(uint16_t value);

std::array<uint8_t, 2> split_16_to_Lo_and_Hi(uint16_t value);
