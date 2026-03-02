// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Utilities
///
/// \file   ulf/mx1bin/utility.hpp
/// \author Jonas Gahlert
/// \date   16/07/2025

#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <ztl/ztl.hpp>

namespace ulf::mx1bin::detail {

// Control characters
inline constexpr uint8_t soh{0x01u};
inline constexpr uint8_t eot{0x17u};
inline constexpr uint8_t dle{0x10u};

inline constexpr uint8_t cypher{0x20u};

/// Is control character
///
/// \param [in] c char
/// \retval true  Is control character
/// \retval false Not control character
constexpr bool is_control_char(uint8_t c) {
  return c == soh || c == eot || c == dle;
}

/// Encode character if applicable
///
/// \tparam OutputIt Output iterator
/// \param [in] c    Char
/// \param [in] out  Output iterator
/// \return   Output iterator
template<std::output_iterator<uint8_t> OutputIt>
[[maybe_unused]] constexpr auto encode_8(uint8_t const c, OutputIt& out) {
  if (is_control_char(c)) {
    *out++ = detail::dle;
    *out++ = c ^ cypher;
  } else *out++ = c;
  return out;
}

/// Encode 2 byte value if applicable
///
/// \tparam OutputIt Output iterator
/// \param v         Value
/// \param out       Output iterator
/// \return   Output iterator
template<std::output_iterator<uint8_t> OutputIt>
[[maybe_unused]] constexpr auto encode_16(uint16_t const v, OutputIt& out) {
  out = encode_8(static_cast<uint8_t>((v & 0xFF00u) >> 8u), out);
  return encode_8(static_cast<uint8_t>((v & 0x00FFu) >> 0u), out);
}

/// Decode character if applicable
///
/// \tparam InputIt Input iterator
/// \param in Input iterator
/// \return  Decoded char
template<std::input_iterator InputIt>
requires(sizeof(std::iter_value_t<InputIt>) == 1uz)
[[maybe_unused]] constexpr auto decode_8(InputIt& in) {
  if (*in == dle) {
    in++;
    return static_cast<uint8_t>(*in++ ^ cypher);
  } else return static_cast<uint8_t>(*in++);
}

/// Decode 2 byte value if applicable
///
/// \tparam InputIt Input iterator
/// \param v        Value
/// \param in       Input iterator
/// \return  Decoded value
template<std::input_iterator InputIt>
requires(sizeof(std::iter_value_t<InputIt>) == 1uz)
[[maybe_unused]] constexpr auto decode_16(InputIt& in) {
  return static_cast<uint16_t>(decode_8(in) << 8u | decode_8(in) << 0u);
}

} // namespace ulf::mx1bin::detail
