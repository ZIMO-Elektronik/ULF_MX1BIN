// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// CRC8 calculation
///
/// \file   ulf/mx1bin/crc8.hpp
/// \author Jonas Gahlert
/// \date   16/07/2025

#pragma once

#include <cstdint>
#include <numeric>
#include <span>

namespace ulf::mx1bin::detail {

/// Calculate CRC8 (Dallas/Maxim)
///
/// The polynomial representations is 0x31.
///
/// \param  byte  Next byte for CRC calculation
/// \return CRC8
constexpr uint8_t crc8(uint8_t byte) {
  uint8_t crc{};
  if (byte & 0x01u) crc ^= 0x5Eu;
  if (byte & 0x02u) crc ^= 0xBCu;
  if (byte & 0x04u) crc ^= 0x61u;
  if (byte & 0x08u) crc ^= 0xC2u;
  if (byte & 0x10u) crc ^= 0x9Du;
  if (byte & 0x20u) crc ^= 0x23u;
  if (byte & 0x40u) crc ^= 0x46u;
  if (byte & 0x80u) crc ^= 0x8Cu;
  return crc;
}

/// Calculate CRC8 (Dallas/Maxim)
///
/// The polynomial representations is 0x31.
///
/// \param  bytes Bytes to calculate CRC8 for
/// \return CRC8
constexpr uint8_t crc8(std::span<uint8_t const> bytes) {
  return std::accumulate(
    cbegin(bytes),
    cend(bytes),
    static_cast<uint8_t>(0u),
    [](uint8_t a, uint8_t b) { return crc8(static_cast<uint8_t>(a ^ b)); });
}

} // namespace ulf::mx1bin::detail
