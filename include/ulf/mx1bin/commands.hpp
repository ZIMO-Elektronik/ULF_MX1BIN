// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// MX1Bin commands
///
/// \file   ulf/mx1bin/commands.hpp
/// \author Jonas Gahlert
/// \date   16/07/2025

#pragma once

#include <cstdint>

namespace ulf::mx1bin {

enum class Command : uint8_t {
  Reset = 0u,                         //
  Nak = 1u,                           //
  TrackControl = 2u,                  // Track Control
  LocoControl = 3u,                   // Loco Control
  InvertFunctionBits = 4u,            //
  Acceleration = 5u,                  //
  ShuttleTrain = 6u,                  // Shuttle Train
  AccessoryControl = 7u,              // Accessory Control
  LocoMemoryQuery = 8u,               // Loco Memory Query
  AccessoryMemoryQuery = 9u,          // Accessory Memory Query
  AddressControl = 10u,               // Decoder Address Control
  CommandStationIOQuery = 11u,        // CommandStation IO state Query
  CommandStationCvManip = 12u,        // CommandStation CV read / write
  CommandStationEquipmentQuery = 13u, // CommandStation Equipment Query
  SerialInfo = 17u,                   // Serial Info
  DecoderCvManip = 19u,               // Cv Manipulation
  LocoMemoryUpdate = 255u,            // Loco Memory Update
  AccessoryMemoryUpdate = 254u,       // Accessory Memory Update
};

} // namespace ulf::mx1bin
