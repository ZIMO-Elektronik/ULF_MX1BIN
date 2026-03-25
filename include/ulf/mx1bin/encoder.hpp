// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// MX1Bin frame 2 stream encoder
///
/// \file   ulf/mx1bin/encoder.hpp
/// \author Jonas Gahlert
/// \date   02/09/2025

#pragma once

#include <concepts>
#include <span>
#include <ztl/ztl.hpp>
#include "utility.hpp"

namespace ulf::mx1bin {

/// Is implicitly or explicitly convertible to
template<typename _From, typename _To>
concept convertible_to = std::is_convertible_v<_From, _To> ||
                         requires { static_cast<_To>(std::declval<_From>()); };

template<typename T, typename To>
concept ConvertibleTo = convertible_to<T, To>;

/// Optional and is convertible to
template<typename T, typename To>
concept OptionalConvertibleTo =
  std::same_as<std::optional<typename std::remove_cvref_t<T>::value_type>,
               typename std::remove_cvref_t<T>> &&
  ConvertibleTo<typename std::remove_cvref_t<T>::value_type, To>;

/// Field encoder
template<typename T>
concept FieldEncoder = requires(T t, uint8_t const u8, uint16_t const u16) {
  { t.uint8(u8) } -> std::same_as<void>;
  { t.uint16(u16) } -> std::same_as<void>;
};

/// Optional field encoder
template<typename T>
concept OptionalFieldEncoder = requires(
  T t, std::optional<uint8_t> const o_u8, std::optional<uint16_t> const o_u16) {
  { t.uint8(o_u8) } -> std::same_as<void>;
  { t.uint16(o_u16) } -> std::same_as<void>;
};

/// Is Encoder
template<typename T>
concept IsEncoder = FieldEncoder<T> && OptionalFieldEncoder<T>;

/// MX1Bin message stream encoder
template<std::ranges::output_range<uint8_t> R>
requires std::constructible_from<std::back_insert_iterator<R>, R&>
struct Encoder {
  // Construct
  Encoder(R& r) : _iter{std::back_inserter(r)} {}

  Encoder(std::back_insert_iterator<R> iter) : _iter{iter} {}

  void addSOF() {
    *_iter++ = detail::soh;
    *_iter++ = detail::soh;
  }

  void addEOT() { *_iter++ = detail::eot; }

  /// Encode uint8
  ///
  /// \tparam T Type
  /// \param  t Value
  /// \warning UB if out of space
  template<typename T>
  requires ConvertibleTo<T, uint8_t>
  void uint8(T const t) {
    auto const v{static_cast<uint8_t>(t)};
    if (detail::is_control_char(v)) {
      *_iter++ = detail::dle;
      *_iter++ = v ^ detail::cypher;
    } else *_iter++ = v;
  }

  /// Overloaded Encode uint8
  ///
  /// \tparam T Optional type
  /// \param  v Optional value
  /// \see Encoder::uint8(uint8_t)
  template<typename T>
  requires OptionalConvertibleTo<T, uint8_t>
  void uint8(T&& t) {
    if (t) return uint8(static_cast<uint8_t>(*t));
  }

  /// Encode uint16
  ///
  /// \tparam T Type
  /// \param  t Value
  /// \warning UB if out of space
  template<typename T>
  requires ConvertibleTo<T, uint16_t>
  void uint16(T const t) {
    auto const v{static_cast<uint16_t>(t)};
    uint8(static_cast<uint8_t>((v & 0xFF00u) >> 8u));
    uint8(static_cast<uint8_t>((v & 0x00FFu) >> 0u));
  }

  /// Overloaded Encode uint16
  ///
  /// \tparam T Optional type
  /// \param  v Optional value
  /// \see Encoder::uint16(uint16_t)
  template<typename T>
  requires OptionalConvertibleTo<T, uint16_t>
  void uint16(T&& v) {
    if (v) return uint16(*v);
  }

private:
  std::back_insert_iterator<R> _iter; ///> Iterator
};

} // namespace ulf::mx1bin
