// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// MX1Bin common types
///
/// \file   ulf/mx1bin/types.hpp
/// \author Jonas Gahlert
/// \date   02/10/2025

#pragma once

#include <cstdint>

namespace ulf::mx1bin {

/// DCC speed step
///
/// \note For more info, see chapter 4.6 in the docs
enum class SpeedStep : uint8_t {
  NA = 0b00,  ///< Last used / Not used
  _14 = 0b01, ///< 14 Speed Steps
  _28 = 0b10, ///< 28 Speed Steps
  _126 = 0b11 ///< 126 Speed Steps
};

} // namespace ulf::mx1bin
