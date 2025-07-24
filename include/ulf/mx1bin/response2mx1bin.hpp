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

using Packet = ztl::inplace_vector<uint8_t, 40u>;

constexpr Packet response2mx1bin(Response re) {
  Packet result{};
  auto iter{begin(result)};
  *iter++ = detail::soh;
  *iter++ = detail::soh;
  bool long_frame{false};

  if (std::holds_alternative<Ack>(re)) {
    std::get<Ack>(re).encode(iter);
  } else if (std::holds_alternative<Nak>(re)) {
    std::get<Nak>(re).encode(iter);
  } else if (std::holds_alternative<TrackControlReply>(re)) {
    std::get<TrackControlReply>(re).encode(iter);
  } else if (std::holds_alternative<DecoderControlReply>(re)) {
    std::get<DecoderControlReply>(re).encode(iter);
  } else if (std::holds_alternative<InvertFunctionBitsReply>(re)) {
    std::get<InvertFunctionBitsReply>(re).encode(iter);
  } else if (std::holds_alternative<ShuttleTrainReply>(re)) {
    std::get<ShuttleTrainReply>(re).encode(iter);
  } else if (std::holds_alternative<AccessoryReply>(re)) {
    std::get<AccessoryReply>(re).encode(iter);
  } else if (std::holds_alternative<LocoMemoryQueryReply>(re)) {
    std::get<LocoMemoryQueryReply>(re).encode(iter);
  } else if (std::holds_alternative<AccessoryMemoryQueryReply>(re)) {
    std::get<AccessoryMemoryQueryReply>(re).encode(iter);
  } else if (std::holds_alternative<AddressControlReply>(re)) {
    std::get<AddressControlReply>(re).encode(iter);
  } else if (std::holds_alternative<CommandStationIOQueryReply>(re)) {
    std::get<CommandStationIOQueryReply>(re).encode(iter);
  } else if (std::holds_alternative<CommandStationCvManipReply>(re)) {
    std::get<CommandStationCvManipReply>(re).encode(iter);
  } else if (std::holds_alternative<CommandStationEquipmentQueryReply>(re)) {
    std::get<CommandStationEquipmentQueryReply>(re).encode(iter);
    long_frame = true;
  } else if (std::holds_alternative<DecoderCvManipReply>(re)) {
    std::get<DecoderCvManipReply>(re).encode(iter);
  } else if (std::holds_alternative<DecoderCvManipErrorReply>(re)) {
    std::get<DecoderCvManipErrorReply>(re).encode(iter);
  } else {
    std::get<DecoderCvManipBusyReply>(re).encode(iter);
  }
  result.resize(static_cast<Packet::size_type>(iter - begin(result)));

  if (!long_frame) {
    *iter++ = crc8(std::span<uint8_t const>{result}.subspan(2uz));
  } else {
    auto crc{crc16(std::span<uint8_t const>{result}.subspan(2uz))};
    *iter++ = static_cast<uint8_t>((crc & 0xFF00u) >> 8u);
    *iter++ = static_cast<uint8_t>((crc & 0x00FFu) >> 0u);
  }

  *iter++ = detail::eot;

  result.resize(static_cast<Packet::size_type>(iter - begin(result)));

  return result;
}

} // namespace ulf::mx1bin
