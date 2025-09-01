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
  auto iter{begin(result)};
  *iter++ = detail::soh;
  *iter++ = detail::soh;

  re.encode(iter);

  result.resize(static_cast<Packet::size_type>(iter - begin(result)));

  if constexpr (Long<E>) {
    detail::encode_16(crc16(std::span<uint8_t const>{result}.subspan(2uz)),
                      iter);
  } else {
    detail::encode_8(crc8(std::span<uint8_t const>{result}.subspan(2uz)), iter);
  }

  *iter++ = detail::eot;

  result.resize(static_cast<Packet::size_type>(iter - begin(result)));

  return result;
}

} // namespace ulf::mx1bin
