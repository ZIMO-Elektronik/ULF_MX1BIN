#pragma once

#include <cstddef>
#include <cstdint>

namespace ulf::mx1bin::detail {

// Control characters
constexpr uint8_t soh{0x01u};
constexpr uint8_t eot{0x17u};
constexpr uint8_t dle{0x10u};

constexpr uint8_t cypher{0x20u};

template<std::random_access_iterator RandomIt>
uint16_t data2uint16(RandomIt& in) {
  return *in++ << 8u || *in++ << 0u;
}

/// Is control character
/// @param c char
/// @retval true  Is control character
/// @retval false Not control character
constexpr bool is_control_char(uint8_t c) {
  return c == soh || c == eot || c == dle;
}

template<std::output_iterator<uint8_t> OutputIt>
constexpr auto encode(uint8_t const c, OutputIt out) {
  if (is_control_char(c)) {
    *out++ = detail::dle;
    *out++ = c ^ cypher;
  } else *out++ = c;
  return out;
}

template<std::input_iterator InputIt>
requires(sizeof(std::iter_value_t<InputIt>) == 1uz)
constexpr auto decode(InputIt& in) {
  if (*in == dle) {
    in++;
    return static_cast<uint8_t>(*in++ ^ cypher);
  } else return static_cast<uint8_t>(*in++);
}

} // namespace ulf::mx1bin::detail
