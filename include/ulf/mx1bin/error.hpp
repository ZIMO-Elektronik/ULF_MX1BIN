#pragma once

#include <cstdint>

namespace ulf::mx1bin {

enum class Error {
  NO_ERROR = 0x00,
  ERR_ADRESSE = 0x01,
  ERR_INDEX = 0x02,
  ERR_FORWARD = 0x03,
  ERR_BUSY = 0x04,
  ERR_NO_MOT = 0x05,
  ERR_NO_DCC = 0x06,
  ERR_CV_ADRESSE = 0x07,
  ERR_SECTION = 0x08,
  ERR_NO_MODUL = 0x09,
  ERR_MESSAGE = 0x0A,
  ERR_SPEED = 0x0B,
  ERR_ADRESSE_OCUPIED = 0x0C,
};

} // namespace ulf::mx1bin
