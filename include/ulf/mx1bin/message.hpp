// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Message structs
///
/// \file   ulf/mx1bin/message.hpp
/// \author Jonas Gahlert
/// \date   16/07/2025

#pragma once

#include <cstdint>
#include <expected>
#include <optional>
#include <ranges>
#include <span>
#include <system_error>
#include <variant>
#include <ztl/inplace_vector.hpp>
#include "bitfields.hpp"
#include "commands.hpp"
#include "decoder.hpp"
#include "encoder.hpp"
#include "utility.hpp"

#include "message/acceleration.hpp"
#include "message/accessory_control.hpp"
#include "message/accessory_memory_query.hpp"
#include "message/ack.hpp"
#include "message/address_control.hpp"
#include "message/command_station_cv_manip.hpp"
#include "message/command_station_equipment_query.hpp"
#include "message/command_station_io_query.hpp"
#include "message/decoder_cv_manip.hpp"
#include "message/invert_function_bits.hpp"
#include "message/loco_control.hpp"
#include "message/loco_memory_query.hpp"
#include "message/message_base.hpp"
#include "message/nak.hpp"
#include "message/reset.hpp"
#include "message/serial_info.hpp"
#include "message/shuttle_tain.hpp"
#include "message/track_control.hpp"

namespace ulf::mx1bin {

/// Encodable concept
template<typename T, typename E = StreamEncoder<std::vector<uint8_t>>>
concept Encodable = requires {
  { &T::template encode<E> } -> std::same_as<E (T::*)(E) const>;
};

/// Decodable concept
///
/// \note This is safe, since return is checked against the actual type
template<typename T>
concept Decodable = requires(T t, StreamDecoder<char const*, char const*>& d) {
  { T::decode(d) } -> std::same_as<std::expected<T, std::errc>>;
};

/// Decode helper
///
/// \tparam T Decodable type
/// \tparam R Range type
/// \param r  Input range
/// \retval T         Decoded message
/// \retval std::errc Malformed stream
template<Decodable T, std::ranges::input_range R>
constexpr std::expected<T, std::errc> decode(R const& r) {
  StreamDecoder d(r);
  d.strip();
  return T::decode(d);
}

/// Message
using Message = std::variant<Ack<Command::DecoderCvManip>,
                             Nak,
                             Reset,
                             TrackControl,
                             TrackControl::Reply,
                             LocoControl,
                             LocoControl::Reply,
                             InvertFunctionBits,
                             InvertFunctionBits::Reply,
                             Acceleration,
                             Acceleration::Reply,
                             ShuttleTrain,
                             ShuttleTrain::Reply,
                             AccessoryControl,
                             AccessoryControl::Reply,
                             LocoMemoryQuery,
                             LocoMemoryQuery::Reply,
                             AccessoryMemoryQuery,
                             AccessoryMemoryQuery::Reply,
                             AddressControl,
                             AddressControl::Reply,
                             CommandStationIOQuery,
                             CommandStationIOQuery::Reply,
                             CommandStationCvManip,
                             CommandStationCvManip::Reply,
                             CommandStationEquipmentQuery,
                             CommandStationEquipmentQuery::Reply,
                             SerialInfo,
                             DecoderCvManip,
                             DecoderCvManip::Reply,
                             DecoderCvManip::Busy,
                             DecoderCvManip::Error>;

template<typename T>
concept Long = requires(T t) {
  requires std::same_as<std::remove_cv_t<std::remove_reference_t<T>>,
                        CommandStationEquipmentQuery::Reply>;
};

template<typename T>
concept Short = requires(T t) { requires !Long<T>; };

using Packet = ztl::inplace_vector<uint8_t, sizeof(Message) * 2u>;

} // namespace ulf::mx1bin
