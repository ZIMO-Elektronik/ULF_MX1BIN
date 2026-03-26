// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Loco Memory Query Message
///
/// \file   ulf/mx1bin/message/loco_memory_query.hpp
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

struct LocoMemoryQuery {
  struct Reply {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::L1Ack,
                                      Sender::CommandStation,
                                      StationType::MX1},
              .code = Command::LocoMemoryQuery};
    Error error{Error::NO_ERROR};
    bitfields::DecoderAddress cAdr{}; ///< Address
    bitfields::ControlSpeed cSpeed{}; ///< Speed
    bitfields::ControlData cData1{};  ///< Control data
    uint8_t cData2{};                 ///< F1..8
    uint8_t cData3{};                 ///< F9..12
    uint8_t cAzBz{};                  ///< Accel / Break time
    uint8_t cStatus{};                ///< Active / inactive
    uint8_t cData4{};                 ///< F13..20
    uint8_t cData5{};                 ///< F21..28
    template<Encoder E>
    E encode(E e) const {
      return head.encode(e)
        .uint8(std::to_underlying(error))
        .uint16(cAdr)
        .uint8(cSpeed)
        .uint8(cData1)
        .uint8(cData2)
        .uint8(cData3)
        .uint8(cAzBz)
        .uint8(cStatus)
        .uint8(cData4)
        .uint8(cData5);
    }
    template<Decoder D>
    static std::expected<Reply, std::errc> decode(D& d) {
      auto const head{Head::decode(d)};
      if (!head ||
          !d.has_at_least(sizeof(cAdr) + sizeof(cSpeed) + sizeof(cData1) +
                          sizeof(cData2) + sizeof(cData3) + sizeof(cAzBz) +
                          sizeof(cStatus) + sizeof(cData4) + sizeof(cData5)))
        return std::unexpected(std::errc::invalid_argument);
      return Reply{.head = *head,
                   .error = static_cast<Error>(d.uint8()),
                   .cAdr = d.uint16(),
                   .cSpeed = d.uint8(),
                   .cData1 = d.uint8(),
                   .cData2 = d.uint8(),
                   .cData3 = d.uint8(),
                   .cAzBz = d.uint8(),
                   .cStatus = d.uint8(),
                   .cData4 = d.uint8(),
                   .cData5 = d.uint8()};
    }
  };
  using Head = detail::Head;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    Sender::CommandStation,
                                    StationType::MX1},
            .code = Command::LocoMemoryQuery};
  bitfields::DecoderAddress cAdr{}; ///< Address
  template<Encoder E>
  E encode(E e) const {
    return head.encode(e).uint16(cAdr);
  }
  template<Decoder D>
  static std::expected<LocoMemoryQuery, std::errc> decode(D& d) {
    auto const head{Head::decode(d)};
    if (!head || !d.has_at_least(sizeof(cAdr)))
      return std::unexpected(std::errc::invalid_argument);
    return LocoMemoryQuery{.head = *head, .cAdr = d.uint16()};
  }
};
} // namespace ulf::mx1bin
