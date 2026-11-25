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
#include "concepts.hpp"

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
  out = encode_8(static_cast<uint8_t>(v >> 8u), out);
  return encode_8(static_cast<uint8_t>(v >> 0u), out);
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
[[maybe_unused]] constexpr uint16_t decode_16(InputIt& in) {
  return static_cast<uint16_t>(static_cast<uint32_t>(decode_8(in)) << 8u |
                               static_cast<uint32_t>(decode_8(in)) << 0u);
}

/**
 * Calculates the unencoded size of a data type within a stream
 *
 * @tparam T Type to calculate size
 *
 * @return Size of type in stream
 */
template<typename T>
[[maybe_unused]] constexpr auto stream_size() {
  if constexpr (concepts::trivial_data<T> || concepts::optional_trivial_data<T>)
    if constexpr (concepts::detail::is_optional<T>)
      return sizeof(concepts::detail::remove_optional_t<T>);
    else return sizeof(T);
  else if constexpr (concepts::strong_enum<T, std::uint8_t> ||
                     concepts::optional_strong_enum<T, std::uint8_t> ||
                     concepts::bitfield<T, std::uint8_t> ||
                     concepts::optional_bitfield<T, std::uint8_t>)
    return sizeof(std::uint8_t);
  else if constexpr (concepts::strong_enum<T, std::uint16_t> ||
                     concepts::optional_strong_enum<T, std::uint16_t> ||
                     concepts::bitfield<T, std::uint16_t> ||
                     concepts::optional_bitfield<T, std::uint16_t>)
    return sizeof(std::uint16_t);
  else std::unreachable();
}

/**
 * Calculates the required, unencoded, size of a data type within a stream
 *
 * \note  Optional data types count as `zero`
 *
 * @tparam T Type to calculate size
 *
 * @return Size of type in stream
 */
template<typename T>
[[maybe_unused]] constexpr auto required_stream_size() {
  if constexpr (concepts::detail::is_optional<T>) return 0uz;
  else return stream_size<T>();
}

} // namespace ulf::mx1bin::detail
