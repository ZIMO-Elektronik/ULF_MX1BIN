#pragma once

#include <span>
#include "utility.hpp"

namespace ulf::mx1bin::detail {

/// Encoder concept
template<typename T>
concept encoder = requires { typename T::encoder_tag; };

/// MX1Bin message stream encoder
template<std::output_iterator<uint8_t> I, std::sentinel_for<I> S>
struct Encoder {
  // Encoder tag
  using encoder_tag = void;

  // Construct/copy/destroy
  template<std::ranges::input_range R>
  requires std::convertible_to<std::ranges::iterator_t<R>, I> &&
             std::convertible_to<std::ranges::sentinel_t<R>, S>
  Encoder(R&& r) : _iter{std::ranges::begin(r)}, _end{std::ranges::end(r)} {}
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

  /// Overloaded Encode uint8
  ///
  /// @param v Optional value
  /// @see Encoder::uint8(uint8_t)
  inline void uint8(std::optional<uint8_t> v) {
    if (v) return uint8(*v);
  }

  /// Encode uint16
  ///
  /// @warning UB if out of space
  /// @param v Value
  void uint16(uint16_t v) {
    uint8(v & 0xFF00u >> 8u);
    uint8(v & 0x00FFu >> 0u);
  }

  /// Overloaded Encode uint16
  ///
  /// @param v Optional value
  /// @see Encoder::uint16(uint16_t)
  inline void uint16(std::optional<uint16_t> v) {
    if (v) return uint16(*v);
  }

private:
  /// Iterator
  I _iter;

  /// End
  S _end;
};

// Deduction guides
template<std::output_iterator<uint8_t> I, std::sentinel_for<I> S>
Encoder(I, S) -> Encoder<I, S>;

template<std::ranges::input_range R>
Encoder(R&&) -> Encoder<decltype(std::ranges::begin(std::declval<R&>())),
                        decltype(std::ranges::end(std::declval<R&>()))>;

} // namespace ulf::mx1bin::detail
