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

namespace ulf::mx1bin::detail {

constexpr ctll::fixed_string pattern{"\x01\x01(.*?)\x17"};

/// Verify frame
///
/// \param frame Potential MX1Bin frame
/// \retval std::errc::invalid_argument Error
/// \retval std::nullopt                Incomplete
/// \retval std::span                   First found frame
constexpr std::expected<std::optional<std::span<uint8_t const>>, std::errc>
verify(std::span<uint8_t const> frame) {
  // Any match
  auto m{ctre::starts_with<pattern>(frame)};
  if (!m) return std::unexpected(std::errc::invalid_argument);
  // Match
  m = ctre::match<pattern>(frame.subspan(0uz, m.size()));
  if (!m) return std::nullopt;
  // CRC
  if (crc8(frame.subspan(2uz, m.size() - 4uz)) ^ frame[m.size() - 2uz])
    return std::unexpected(std::errc::bad_message);
  return frame.subspan(0u, m.size());
}

} // namespace ulf::mx1bin::detail
