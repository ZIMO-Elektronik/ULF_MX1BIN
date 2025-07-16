#pragma once

#include <dcc/dcc.hpp>
#include <expected>
#include <optional>
#include <span>
#include <system_error>
#include "commands.hpp"
#include "frame.hpp"

namespace ulf::mx1bin {

/// MX1Bin 2 DCC
///
/// \param bytes  MX1Bin frame
/// \return       DCC Packet
std::expected<std::optional<dcc::Packet>, std::errc>
mx1bin_2dcc(std::span<uint8_t const> bytes) {
  auto res{detail::verify(bytes)};
  if (!res) return std::unexpected(res.error());
  if (!*res) return std::nullopt;

  auto frame{**res};

  switch (static_cast<Commands>(frame[2])) {
    default: return std::unexpected(std::errc::invalid_argument);
  }

  return {};
}

} // namespace ulf::mx1bin
