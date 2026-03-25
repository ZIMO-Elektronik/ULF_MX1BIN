// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Response format
///
/// \file   ulf/mx1bin/response2mx1bin.hpp
/// \author Jonas Gahlert
/// \date   16/07/2025

#pragma once

#include <ztl/inplace_vector.hpp>
#include "crc16.hpp"
#include "crc8.hpp"
#include "message.hpp"

namespace ulf::mx1bin {

template<Encodable E>
constexpr Packet response2mx1bin(E& re) {
  Packet result{};
  StreamEncoder e{result};
  e.addSOF();

  e = re.encode(e);

  if constexpr (Long<E>) {
    e.uint16(crc16(std::span<uint8_t const>{result}.subspan(2uz)));
  } else {
    e.uint8(crc8(std::span<uint8_t const>{result}.subspan(2uz)));
  }

  e.addEOT();

  return result;
}

} // namespace ulf::mx1bin
