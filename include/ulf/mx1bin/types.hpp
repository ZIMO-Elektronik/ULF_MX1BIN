#pragma once

#include "cstdint"

namespace ulf::mx1bin {

enum class SpeedStep : uint8_t {
  NA = 0b00,
  _14 = 0b01,
  _28 = 0b10,
  _126 = 0b11
};

} // namespace ulf::mx1bin
