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
#include "concepts.hpp"
#include "utility.hpp"

namespace ulf::mx1bin::neww {

template<std::ranges::output_range<uint8_t> R>
struct NewEncoder {
  NewEncoder(R& r) : _iter{std::back_inserter(r)} {}

  NewEncoder& addSOF() {
    *_iter++ = detail::soh;
    *_iter++ = detail::soh;
    return *this;
  }

  NewEncoder& addEOT() {
    *_iter++ = detail::eot;
    return *this;
  }

  constexpr auto encode(uint8_t val) { encode_impl(val); }
  constexpr auto encode(uint16_t val) { encode_impl(val); }

  template<class T>
  constexpr auto encode(std::optional<T> const val) {
    if (val) encode(*val);
  }

  template<class T>
  requires concepts::strong_enum<T, std::uint8_t> ||
           concepts::strong_enum<T, std::uint16_t>
  constexpr auto encode(T x) {
    encode_impl(static_cast<std::underlying_type_t<T>>(x));
  }

  template<class T>
  requires concepts::bitfield<T, std::uint8_t> ||
           concepts::bitfield<T, std::uint16_t>
  constexpr auto encode(T const& x) {
    if constexpr (concepts::bitfield<T, std::uint8_t>)
      encode_impl(static_cast<std::uint8_t>(x));
    else if constexpr (concepts::bitfield<T, std::uint16_t>)
      encode_impl(static_cast<std::uint16_t>(x));
    else std::unreachable();
  }

  template<typename... Args>
  constexpr auto encode(Args... args) {
    return (encode(args), ...);
  }

private:
  constexpr NewEncoder& encode_impl(uint8_t val) {
    auto const v{static_cast<uint8_t>(val)};
    if (detail::is_control_char(v)) {
      *_iter++ = detail::dle;
      *_iter++ = v ^ detail::cypher;
    } else *_iter++ = v;
    return *this;
  }
  constexpr NewEncoder& encode_impl(uint16_t val) {
    auto const v{static_cast<uint16_t>(val)};
    encode_impl(static_cast<uint8_t>(v >> 8u));
    encode_impl(static_cast<uint8_t>(v >> 0u));
    return *this;
  }

  std::back_insert_iterator<R> _iter; ///< Iterator
};

template<typename T, std::ranges::output_range<uint8_t> R>
constexpr void encode(T const& obj, NewEncoder<R>& encoder) {
  std::apply([&](auto... member) { (encoder.encode(obj.*member), ...); },
             T::members::members);
}

template<typename T, std::ranges::output_range<uint8_t> R>
constexpr void encode(T const& obj, R& r) {
  NewEncoder<R> encoder{r};

  encode(obj.head, encoder);
  encode(obj, encoder);
}

} // namespace ulf::mx1bin::neww
