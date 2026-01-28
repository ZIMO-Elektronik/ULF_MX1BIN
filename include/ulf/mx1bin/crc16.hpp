// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// MCRC16 calculation
///
/// \file   ulf/mx1bin/crc16.hpp
/// \author Jonas Gahlert
/// \date   24/07/2025

#pragma once

#include <cstdint>
#include <span>
#include "crc8.hpp"
#include "utility.hpp"

namespace ulf::mx1bin {

namespace detail {

/// Polynomial representation for CRC
constexpr uint16_t poly{0x1021u};

/// Condition mask
constexpr uint16_t mask{0x8000u};

} // namespace detail

/// CRC16 CCITT with polynomial representation 0x1021
struct CRC16 : detail::CRCBase<uint16_t, 0xFFFFu> {
  constexpr void next(uint8_t byte) {
    for (uint8_t i{0x80u}; i; i >>= 1) {
      auto flag{static_cast<bool>(_crc & detail::mask)};
      _crc = static_cast<decltype(_crc)>(_crc << 1u);
      if (byte & i) flag = !flag;
      if (flag) _crc ^= detail::poly;
    }
  }

  constexpr void next(std::span<uint8_t const> bytes) {
    std::ranges::for_each(bytes, [this](uint8_t byte) { next(byte); });
  }
};

/// Calculate CRC16 (CCITT)
///
/// The polynomial representations is 0x1021.
///
/// \param  bytes Bytes to calculate CRC16 for
/// \return CRC16
constexpr uint16_t crc16(std::span<uint8_t const> bytes) {
  CRC16 crc;
  auto iter{cbegin(bytes)};
  while (iter != cend(bytes)) { crc.next(detail::decode_8(iter)); }
  return crc;
}

} // namespace ulf::mx1bin
