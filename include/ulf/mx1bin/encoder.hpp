// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// MX1Bin frame 2 stream encoder
///
/// \file   ulf/mx1bin/encoder.hpp
/// \author Jonas Gahlert
/// \date   02/09/2025

#pragma once

#include <span>
#include <ztl/ztl.hpp>
#include "utility.hpp"

namespace ulf::mx1bin {

template<typename T>
struct is_std_optional : std::false_type {};

/// Is std::optional trait
/// \tparam T Type
template<typename T>
struct is_std_optional<std::optional<T>> : std::true_type {};

template<typename T>
struct optional_inner_type {};

/// Inner type of optional
/// \tparam T Type
template<typename T>
struct optional_inner_type<std::optional<T>> {
  using type = T;
};

/// Is implicitly or explicitly convertible to
template<typename _From, typename _To>
concept convertible_to = std::is_convertible_v<_From, _To> ||
                         requires { static_cast<_To>(std::declval<_From>()); };

template<typename T, typename To>
concept ConvertibleTo = convertible_to<T, To>;

/// Optional and is convertible to
template<typename T, typename To>
concept OptionalConvertibleTo =
  is_std_optional<std::remove_cvref_t<T>>::value &&
  convertible_to<typename optional_inner_type<std::remove_cvref_t<T>>::type,
                 To>;

/// Encoder concept
template<typename T>
concept encoder = requires { typename T::encoder_tag; };

/// MX1Bin message stream encoder
template<std::output_iterator<uint8_t> I, std::sentinel_for<I> S>
struct Encoder {
  // Encoder tag
  using encoder_tag = void;

  // Construct
  template<std::ranges::input_range R>
  requires std::convertible_to<std::ranges::iterator_t<R>, I> &&
             std::convertible_to<std::ranges::sentinel_t<R>, S>
  Encoder(R&& r)
    : _iter{std::ranges::begin(r)}, _begin{std::ranges::begin(r)},
      _end{std::ranges::end(r)} {}
  Encoder(I iter, S end) : _iter{iter}, _begin{iter}, _end{end} {}

  void addSOF() {
    *_iter++ = detail::soh;
    *_iter++ = detail::soh;
  }

  void addEOT() { *_iter++ = detail::eot; }

  size_t difference() {
    return static_cast<size_t>(std::distance(_begin, _iter));
  }

  /// Encode uint8
  ///
  /// \tparam T Type
  /// \param  t Value
  /// \warning UB if out of space
  template<typename T>
  requires ConvertibleTo<T, uint8_t>
  inline void uint8(T const t) {
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
  inline void uint8(T&& t) {
    if (t) return uint8(static_cast<uint8_t>(*t));
  }

  /// Encode uint16
  ///
  /// \tparam T Type
  /// \param  t Value
  /// \warning UB if out of space
  template<typename T>
  requires ConvertibleTo<T, uint16_t>
  inline void uint16(T const t) {
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
  inline void uint16(T&& v) {
    if (v) return uint16(*v);
  }

private:
  I _iter;  ///> Iterator
  I _begin; ///> Begin
  S _end;   ///> End
};

// Deduction guides
template<std::output_iterator<uint8_t> I, std::sentinel_for<I> S>
Encoder(I, S) -> Encoder<I, S>;

template<std::ranges::input_range R>
Encoder(R&&) -> Encoder<decltype(std::ranges::begin(std::declval<R&>())),
                        decltype(std::ranges::end(std::declval<R&>()))>;

} // namespace ulf::mx1bin
