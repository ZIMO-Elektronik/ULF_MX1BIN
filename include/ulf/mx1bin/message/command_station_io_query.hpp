// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Command Station IO Query Message
///
/// \file   ulf/mx1bin/message/command_station_io_query.hpp
/// \author Jonas Gahlert
/// \date   26/03/2026

#pragma once

#include <cstdint>
#include "../bitfields.hpp"
#include "../commands.hpp"
#include "../decoder.hpp"
#include "../encoder.hpp"
#include "../error.hpp"
#include "message_base.hpp"

namespace ulf::mx1bin {

struct CommandStationIOQuery {
  struct Reply {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::L1Ack,
                                      detail::defaultSender,
                                      StationType::MX1},
              .code = Command::CommandStationIOQuery};
    uint8_t values{};
    uint16_t cCurrent1{};
    uint8_t cVoltage1{};
    uint16_t cCurrent2{};
    uint8_t cVoltage2{};
    uint8_t cAux{};
    template<Encoder E>
    E encode(E e) const {
      return head.encode(e)
        .uint8(values)
        .uint16(cCurrent1)
        .uint8(cVoltage1)
        .uint16(cCurrent2)
        .uint8(cVoltage2)
        .uint8(cAux);
    }
    template<Decoder D>
    static std::expected<Reply, std::errc> decode(D& d) {
      auto const head{Head::decode(d)};
      if (!head || !d.has_at_least(sizeof(values) + sizeof(cCurrent1) +
                                   sizeof(cVoltage1) + sizeof(cCurrent2) +
                                   sizeof(cVoltage2) + sizeof(cAux)))
        return std::unexpected{std::errc::invalid_argument};
      return Reply{.head = *head,
                   .values = d.uint8(),
                   .cCurrent1 = d.uint16(),
                   .cVoltage1 = d.uint8(),
                   .cCurrent2 = d.uint16(),
                   .cVoltage2 = d.uint8(),
                   .cAux = d.uint8()};
    }
  };
  using Head = detail::Head;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    detail::defaultSender,
                                    StationType::MX1},
            .code = Command::CommandStationIOQuery};
  uint8_t const zero{};
  template<Encoder E>
  E encode(E e) const {
    return head.encode(e).uint8(zero);
  }
  template<Decoder D>
  static std::expected<CommandStationIOQuery, std::errc> decode(D& d) {
    auto const head{Head::decode(d)};
    if (!head || !d.has_at_least(sizeof(zero)))
      return std::unexpected(std::errc::invalid_argument);
    return CommandStationIOQuery{.head = *head, .zero = d.uint8()};
  }
};

} // namespace ulf::mx1bin
