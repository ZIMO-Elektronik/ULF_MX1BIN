// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Accessory Memory Query Message
///
/// \file   ulf/mx1bin/message/accessory_memory_query.hpp
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

struct AccessoryMemoryQuery {
  struct Reply {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::L1Ack,
                                      Sender::CommandStation,
                                      StationType::MX1},
              .code = Command::AccessoryMemoryQuery};
    Error error{Error::NO_ERROR};     ///< Error
    bitfields::DecoderAddress cAdr{}; ///< Address
    uint8_t cPair{};                  ///<
    uint8_t cOutputs{};               ///< Acc decoder outputs
    template<Encoder E>
    E encode(E e) const {
      return head.encode(e)
        .uint8(std::to_underlying(error))
        .uint16(cAdr)
        .uint8(cPair)
        .uint8(cOutputs);
    }
    template<Decoder D>
    static std::expected<Reply, std::errc> decode(D& d) {
      auto const head{Head::decode(d)};
      if (!head || !d.has_at_least(sizeof(error) + sizeof(cAdr) +
                                   sizeof(cPair) + sizeof(cOutputs)))
        return std::unexpected{std::errc::invalid_argument};
      return Reply{.head = *head,
                   .error = static_cast<Error>(d.uint8()),
                   .cAdr = d.uint16(),
                   .cPair = d.uint8(),
                   .cOutputs = d.uint8()};
    }
  };
  using Head = detail::Head;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    Sender::CommandStation,
                                    StationType::MX1},
            .code = Command::AccessoryMemoryQuery};
  bitfields::DecoderAddress cAdr{}; ///< Address
  template<Encoder E>
  E encode(E e) const {
    return head.encode(e).uint16(cAdr);
  }
  template<Decoder D>
  static std::expected<AccessoryMemoryQuery, std::errc> decode(D& d) {
    auto const head{Head::decode(d)};
    if (!head || !d.has_at_least(sizeof(cAdr)))
      return std::unexpected(std::errc::invalid_argument);
    return AccessoryMemoryQuery{.head = *head, .cAdr = d.uint16()};
  }
};

} // namespace ulf::mx1bin
