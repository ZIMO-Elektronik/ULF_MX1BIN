// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// MX1Bin stream 2 frame decoder
///
/// \file   ulf/mx1bin/decoder.hpp
/// \author Jonas Gahlert
/// \date   02/09/2025

#pragma once

#include <cassert>
#include <iostream>
#include <span>
#include "concepts.hpp"
#include "fields.hpp"
#include "utility.hpp"

namespace ulf::mx1bin::neww {

template<std::input_iterator I, std::sentinel_for<I> S>
struct NewDecoder {
  template<std::ranges::input_range R>
  NewDecoder(R const& r)
    : _iter{std::ranges::cbegin(r)}, _end{std::ranges::cend(r)} {}

  /// -- Required fields --

  template<concepts::is_trivial_data T>
  T decode() {
    if constexpr (std::same_as<uint8_t, T>) {
      std::cout << "Decode U8" << std::endl;
      return u8();
    } else if constexpr (std::same_as<uint16_t, T>) {
      std::cout << "Decode U16" << std::endl;
      return u16();
    } else std::unreachable();
  }

  template<class T>
  requires concepts::strong_enum<T, std::uint8_t> ||
           concepts::strong_enum<T, std::uint16_t>
  auto decode() {
    return static_cast<T>(decode<std::underlying_type_t<T>>());
  }

  template<class T>
  requires concepts::bitfield<T, std::uint8_t> ||
           concepts::bitfield<T, std::uint16_t>
  auto decode() {
    if constexpr (concepts::bitfield<T, std::uint8_t>)
      return T{decode<uint8_t>()};
    else if constexpr (concepts::bitfield<T, std::uint16_t>)
      return T{decode<uint16_t>()};
    else std::unreachable();
  }

  /// -- Optional fields --

  template<typename T>
  requires concepts::detail::is_optional<T> &&
           concepts::is_optional_trivial_data<T>
  T decode() {
    if constexpr (std::same_as<concepts::detail::remove_optional_t<T>,
                               uint8_t>) {
      std::cout << "Decode Optional U8" << std::endl;
      return u8_();
    } else if constexpr (std::same_as<concepts::detail::remove_optional_t<T>,
                                      uint16_t>) {
      std::cout << "Decode Optional U16" << std::endl;
      return u16_();
    } else std::unreachable();
  }

  template<typename T>
  requires concepts::optional_strong_enum<T, std::uint8_t> ||
           concepts::optional_strong_enum<T, std::uint16_t>
  auto decode() {
    return static_cast<concepts::detail::remove_optional_t<T>>(
      decode<std::optional<
        std::underlying_type_t<concepts::detail::remove_optional_t<T>>>>());
  }

  template<class T>
  requires concepts::optional_bitfield<T, uint8_t> ||
           concepts::optional_bitfield<T, uint16_t>
  auto decode() {
    if constexpr (concepts::optional_bitfield<T, uint8_t>)
      return T{decode<std::optional<std::uint8_t>>()};
    else if constexpr (concepts::optional_bitfield<T, uint16_t>)
      return T{decode<std::optional<std::uint16_t>>()};
    else std::unreachable();
  }

private:
  /// Decode next uint8
  ///
  /// \warning  UB if out of data
  /// \return   Decoded value
  uint8_t u8() {
    if (*_iter == detail::dle) {
      // Encoded
      _iter++;
      return static_cast<uint8_t>(*_iter++ ^ detail::cypher);
    }
    // Non-encoded
    return static_cast<uint8_t>(*_iter++);
  }

  /// Decode next uint8 - checked
  ///
  /// \retval std::nullopt  Out of data
  /// \retval uint8_t       Decoded value
  std::optional<uint8_t> u8_() {
    if (_iter == _end) return std::nullopt;
    if (*_iter == detail::dle) {
      // Encoded
      _iter++;
      return _iter != _end ? std::make_optional(
                               static_cast<uint8_t>(*_iter++ ^ detail::cypher))
                           : std::nullopt;
    }
    // Non-encoded
    return static_cast<uint8_t>(*_iter++);
  }

  /// Decode next uint16
  ///
  /// \warning  UB if out of data
  /// \return   Decoded value
  uint16_t u16() {
    return static_cast<uint16_t>(static_cast<uint32_t>(u8()) << 8u |
                                 static_cast<uint32_t>(u8()) << 0u);
  }

  /// Decode next uint16 - checked
  ///
  /// \retval std::nullopt  Out of data
  /// \retval uint16        Decoded value
  std::optional<uint16_t> u16_() {
    if (auto const hi{u8_()})
      if (auto const lo{u8_()})
        return static_cast<uint16_t>(static_cast<uint32_t>(*hi) << 8u |
                                     static_cast<uint32_t>(*lo) << 0u);
    return std::nullopt;
  }

  I _iter; ///< Iterator
  S _end;  ///< End
};

template<std::ranges::input_range R>
NewDecoder(R const&) -> NewDecoder<decltype(std::declval<R const&>().cbegin()),
                                   decltype(std::declval<R const&>().cend())>;

template<class Message, typename I, typename S>
requires requires {
  typename Message::fields;
  typename Message::Head;
  typename Message::Head::fields;
}
auto decode(NewDecoder<I, S>& decoder) {
  auto head = std::apply(
    [&](auto&&... args) {
      return typename Message::Head{
        decoder.template decode<member_type_t<decltype(args)>>()...};
    },
    Message::Head::fields::members);

  return std::apply(
    [&](auto&&... args) {
      return Message{
        head, decoder.template decode<member_type_t<decltype(args)>>()...};
    },
    Message::fields::members);
}

template<class Message, std::ranges::input_range R>
auto decode(R const& r) {
  NewDecoder decoder{r};
  return decode<Message>(decoder);
}

} // namespace ulf::mx1bin::neww
