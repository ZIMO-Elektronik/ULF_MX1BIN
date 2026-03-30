// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Frame verification
///
/// \file   ulf/mx1bin/frame.hpp
/// \author Jonas Gahlert
/// \date   16/07/2025

#pragma once

#include <cstdint>
#include <ctre.hpp>
#include <expected>
#include <optional>
#include <span>
#include <system_error>
#include "bitfields.hpp"
#include "crc16.hpp"
#include "crc8.hpp"
#include "decoder.hpp"
#include "info.hpp"
#include "utility.hpp"

namespace ulf::mx1bin::detail {

inline constexpr ctll::fixed_string start_pattern{"\x01\x01"};
inline constexpr ctll::fixed_string pattern{"\x01\x01(.*?)\x17"};
inline constexpr ctll::fixed_string end_pattern{"(.*?)\x17"};

/// Verify frame
///
/// \param frame Potential MX1Bin frame
/// \retval std::errc::invalid_argument Error
/// \retval std::nullopt                Incomplete
/// \retval std::span                   First found frame
constexpr std::expected<std::optional<std::span<uint8_t const>>, std::errc>
verify(std::span<uint8_t const> frame) {
  // Check SOF
  auto const start_match(ctre::starts_with<start_pattern>(frame));
  if (!start_match) return std::unexpected(std::errc::invalid_argument);

  // Find message withing stream
  if (auto const match{ctre::match<pattern>(frame)}) {
    // Whole match
    frame = frame.subspan(0, match.size());
  } else {
    // Maybe we simply have too much data, search
    if (auto const search_match{ctre::search<end_pattern>(frame)}) {
      // Found it
      frame = frame.subspan(0, search_match.size());
    } else {
      // Probably insufficient data
      return std::nullopt;
    }
  }

  // Long or Short frame
  StreamDecoder d{frame};
  d.strip();
  d.uint8(); // Skip uSID
  bitfields::Info info{d.uint8()};

  if (info.frameType == FrameType::Long) {
    // Long Frame - CRC16
    auto rit{++frame.rbegin()}; // Skip EOT
    uint16_t crc{};
    for (auto i{0u}; i < sizeof(uint16_t); i++) {
      auto const byte{*rit++};
      if (*rit == dle) {
        crc |= static_cast<uint16_t>((byte ^ cypher) << (8u * i));
        rit++;
      } else {
        crc |= static_cast<uint16_t>(byte << (8u * i));
      }
    }
    if (crc16(frame.subspan(2uz,
                            static_cast<std::size_t>(std::distance(
                              frame.begin() + 2uz, rit.base())))) ^
        crc)
      return std::unexpected(std::errc::bad_message);

  } else {
    // Short Frame - CRC8
    auto rit{++frame.rbegin()}; // Skip EOT
    uint8_t crc{*rit++};
    if (*rit == dle) {
      crc ^= cypher; // Decode CRC
      rit++;         // Adjust Iterator
    }
    if (crc8(frame.subspan(2uz,
                           static_cast<std::size_t>(
                             std::distance(frame.begin() + 2uz, rit.base())))) ^
        crc)
      return std::unexpected(std::errc::bad_message);
  }

  return frame;
}

} // namespace ulf::mx1bin::detail
