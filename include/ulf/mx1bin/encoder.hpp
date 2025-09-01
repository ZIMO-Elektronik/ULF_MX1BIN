#pragma once

#include <span>
#include "utility.hpp"

namespace ulf::mx1bin::detail {

/// Encoder concept
template<typename T>
concept encoder = requires { typename T::encoder_tag; };

/// MX1Bin message stream encoder
template<std::input_iterator I, std::sentinel_for<I> S>
struct Encoder {
  // Encoder tag
  using encoder_tag = void;

  // Construct/copy/destroy
  template<std::ranges::input_range R>
  requires std::convertible_to<std::ranges::iterator_t<R>, I> &&
             std::convertible_to<std::ranges::sentinel_t<R>, S>
  Encoder(R const& r)
    : _iter{std::ranges::begin(r)}, _end{std::ranges::end(r)} {}
  Encoder(I iter, S end) : _iter{iter}, _end{end} {}
  Encoder(Encoder const& d) = default;

  void addSOF() {
    *_iter++ = soh;
    *_iter++ = soh;
  }

  void addEOH() { *_iter++ = eot; }

  /// Encode uint8
  ///
  /// @warning UB if out of space
  /// @param v Value
  void uint8(uint8_t v) {
    if (is_control_char(v)) {
      *_iter++ = dle;
      *_iter++ = v ^ cypher;
    }
    *_iter++ = v;
  }

  /// Encode uint16
  ///
  /// @warning UB if out of space
  /// @param v Value
  void uint16(uint16_t v) {
    uint8(v & 0xFF00u >> 8u);
    uint8(v & 0x00FFu >> 0u);
  }

private:
  /// Iterator
  I _iter;

  /// End
  S _end;
};

// Deduction guides
template<std::input_iterator I, std::sentinel_for<I> S>
Encoder(I, S) -> Encoder<I, S>;

template<std::ranges::input_range R>
Encoder(R&&) -> Encoder<std::ranges::iterator_t<R>, std::ranges::sentinel_t<R>>;

} // namespace ulf::mx1bin::detail
