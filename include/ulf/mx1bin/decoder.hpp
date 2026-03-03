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
#include <span>
#include "utility.hpp"

namespace ulf::mx1bin::detail {

/// Decoder concept
template<typename T>
concept decoder = requires { typename T::decoder_tag; };

/// MX1Bin message stream decoder
template<std::input_iterator I, std::sentinel_for<I> S>
struct Decoder {
  // Decoder tag
  using decoder_tag = void;

  // Construct
  template<std::ranges::input_range R>
  requires std::convertible_to<std::ranges::iterator_t<R>, I> &&
             std::convertible_to<std::ranges::sentinel_t<R>, S>
  Decoder(R const& r)
    : _iter{std::ranges::cbegin(r)}, _end{std::ranges::cend(r)} {}
  Decoder(I iter, S end) : _iter{iter}, _end{end} {}

  /// Strips SOH and EOH
  ///
  /// \return Decoder reference
  Decoder& strip() {
    assert(_iter != _end);
    while (*_iter == soh) _iter++;
    while (*(_end - 1) == eot) _end--;
    return *this;
  }

  /// Decode next uint8
  ///
  /// \warning  UB if out of data
  /// \return   Decoded value
  uint8_t uint8() {
    if (*_iter == dle) {
      // Encoded
      _iter++;
      return static_cast<uint8_t>(*_iter++ ^ cypher);
    }
    // Non-encoded
    return static_cast<uint8_t>(*_iter++);
  }

  /// Decode next uint8 - checked
  ///
  /// \retval std::nullopt  Out of data
  /// \retval uint8_t       Decoded value
  std::optional<uint8_t> s_uint8() {
    if (_iter == _end) return std::nullopt;
    if (*_iter == dle) {
      // Encoded
      _iter++;
      return _iter != _end
               ? std::make_optional(static_cast<uint8_t>(*_iter++ ^ cypher))
               : std::nullopt;
    }
    // Non-encoded
    return std::make_optional(static_cast<uint8_t>(*_iter++));
  }

  /// Decode next uint16
  ///
  /// \warning  UB if out of data
  /// \return   Decoded value
  uint16_t uint16() {
    return static_cast<uint16_t>(uint8() << 8u | uint8() << 0u);
  }

  /// Decode next uint16 - checked
  ///
  /// \retval std::nullopt  Out of data
  /// \retval uint16        Decoded value
  std::optional<uint16_t> s_uint16() {
    if (auto const hi{s_uint8()})
      if (auto const lo{s_uint8()})
        return std::make_optional(static_cast<uint16_t>(*hi << 8u | *lo << 0u));
    return std::nullopt;
  }

  /// Calculate remaining size
  ///
  /// \return Remaining message size
  size_t remaining() const {
    auto iter{_iter};
    size_t size{0uz};
    while (iter != _end) {
      if (*iter++ == dle) {
        // Encoded
        if (iter++ == _end) break; // Decode error
        size += 2uz;
      } else {
        // Non-encoded
        size++;
      }
    }
    return size;
  }

  /// Check, if message has at least n byte left
  ///
  /// \param n Minimum size to check
  /// \retval true  At least n bytes left
  /// \retval false Not enough bytes left
  bool has_at_least(size_t const n) const {
    auto iter{_iter};
    size_t size{0uz};
    while (iter != _end && size < n) {
      if (*iter++ == dle) {
        // Encoded
        if (iter++ == _end) break; // Decode error
        size += 2uz;
      } else {
        // Non-encoded
        size++;
      }
    }
    return size >= n;
  }

private:
  /// Iterator
  I _iter;

  /// End
  S _end;
};

// Deduction guides
template<std::input_iterator I, std::sentinel_for<I> S>
Decoder(I, S) -> Decoder<I, S>;

template<std::ranges::input_range R>
Decoder(R const&) -> Decoder<decltype(std::declval<R const&>().cbegin()),
                             decltype(std::declval<R const&>().cend())>;

} // namespace ulf::mx1bin::detail
