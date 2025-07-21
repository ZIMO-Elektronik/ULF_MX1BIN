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
#include <vector>
#include "utility.hpp"

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
  uint8_t result{0xFFuz};

  auto iter{cbegin(bytes)};
  while (iter != cend(bytes)) { result = crc8(decode(iter) ^ result); }

  return result;
}

} // namespace ulf::mx1bin::detail
