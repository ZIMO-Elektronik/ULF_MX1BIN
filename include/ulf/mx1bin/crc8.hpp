// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// CRC8 calculation
///
/// \file   ulf/mx1bin/crc8.hpp
/// \author Jonas Gahlert
/// \date   16/07/2025

#pragma once

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <span>
#include <vector>
#include "utility.hpp"

namespace ulf::mx1bin {

namespace detail {

/// Base for CRC
///
/// \tparam T     Type of CRC value
/// \tparam Init  Initial value
template<std::unsigned_integral T, T Init>
struct CRCBase {
  constexpr void reset() { _crc = Init; }
  constexpr T value() const { return _crc; }
  constexpr operator T() const { return _crc; }

protected:
  T _crc{Init};
};

} // namespace detail

/// Dallas/Maxim CRC8 with polynomial representation 0x31u
struct CRC8 : detail::CRCBase<uint8_t, 0xFFu> {
  constexpr void next(uint8_t byte) {
    _crc ^= byte;
    uint8_t tmp{};
    if (_crc & 0x01u) tmp ^= 0x5Eu;
    if (_crc & 0x02u) tmp ^= 0xBCu;
    if (_crc & 0x04u) tmp ^= 0x61u;
    if (_crc & 0x08u) tmp ^= 0xC2u;
    if (_crc & 0x10u) tmp ^= 0x9Du;
    if (_crc & 0x20u) tmp ^= 0x23u;
    if (_crc & 0x40u) tmp ^= 0x46u;
    if (_crc & 0x80u) tmp ^= 0x8Cu;
    _crc = tmp;
  }

  constexpr void next(std::span<uint8_t const> bytes) {
    std::ranges::for_each(bytes, [this](uint8_t byte) { next(byte); });
  }
};

static_assert(sizeof(CRC8) == sizeof(uint8_t));

/// Calculate CRC8 (Dallas/Maxim)
///
/// The polynomial representations is 0x31.
///
/// \param  bytes Bytes to calculate CRC8 for
/// \return CRC8
constexpr uint8_t crc8(std::span<uint8_t const> bytes) {
  CRC8 crc;
  auto iter{cbegin(bytes)};
  while (iter != cend(bytes)) { crc.next(detail::decode(iter)); }
  return crc;
}

} // namespace ulf::mx1bin
