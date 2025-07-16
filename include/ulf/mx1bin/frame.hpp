#pragma once

#include <cstdint>
#include <expected>
#include <optional>
#include <span>
#include <system_error>

namespace ulf::mx1bin::detail {

/// Verify frame
///
/// \param frame Potential MX1Bin frame
/// \retval std::errc::invalid_argument Error
/// \retval std::nullopt                Incomplete
/// \retval std::span                   First found frame
std::expected<std::optional<std::span<uint8_t const>>, std::errc>
verify(std::span<uint8_t const> frame) {
  auto const count{size(frame)};
  // Can't be MX1 binary yet
  if (!count) return std::nullopt;
  // Not start of heading (SOH)
  else if ((count >= 1uz && frame[0uz] != '\x01') ||
           (count >= 2uz && frame[1uz] != '\x01'))
    return std::unexpected(std::errc::invalid_argument);
  // Look for end of transmission block (ETB)
  for (auto i{2uz}; i < count; ++i)
    if (frame[i - 1uz] != '\x10' && frame[i] == '\x17')
      return frame.subspan(0uz, i + 1uz);
  // Not enough characters
  return std::nullopt;
}

} // namespace ulf::mx1bin::detail
