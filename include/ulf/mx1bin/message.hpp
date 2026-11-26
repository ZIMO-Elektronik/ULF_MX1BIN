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
#include <vector>
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
#include "message/shuttle_train.hpp"
#include "message/track_control.hpp"

namespace ulf::mx1bin {

namespace detail {
template<typename T>
concept has_head = requires { typename T::Head; } && requires(T t) { t.head; };

template<typename T>
concept has_member_list = requires { typename T::members; };

} // namespace detail

template<typename T>
concept ProtocolMessage = detail::has_head<T> && detail::has_member_list<T>;

template<typename T, typename E = StreamEncoder<std::vector<uint8_t>>>
concept EncodableProtocolMessage =
  ProtocolMessage<T> && requires(T t, E e) { encode(t, e); };

template<typename T, typename D = StreamDecoder<char const*, char const*>>
concept DecodableProtocolMessage =
  ProtocolMessage<T> && requires(T t, D d) { decode<T>(d); };

template<typename T,
         typename E = StreamEncoder<std::vector<uint8_t>>,
         typename D = StreamDecoder<char const*, char const*>>
concept CompatibleProtocolMessage =
  EncodableProtocolMessage<T, E> && DecodableProtocolMessage<T, D>;

/// Long frame message concept
template<typename T>
concept Long = requires(T t) {
  requires std::same_as<std::remove_cv_t<std::remove_reference_t<T>>,
                        CommandStationEquipmentQuery::Reply>;
};

/// Short frame message concept
template<typename T>
concept Short = requires(T t) { requires !Long<T>; };

/// Message
using Message = std::variant<Ack,
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
                             DecoderCvManip::ReplyL2,
                             DecoderCvManip::Busy,
                             DecoderCvManip::Error>;

using Packet = ztl::inplace_vector<uint8_t, sizeof(Message) * 2u>;

} // namespace ulf::mx1bin
