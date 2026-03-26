// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Accessory Control Message
///
/// \file   ulf/mx1bin/message/accessory_control.hpp
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

struct AccessoryControl {
  struct Reply {
    using Head = detail::ReplyHead<Command::AccessoryControl>;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::L1Ack,
                                      Sender::CommandStation,
                                      StationType::MX1}};
    Error error{Error::NO_ERROR};
    bitfields::ControlPayload payload{};
    template<Encoder E>
    E encode(E e) const {
      return head.encode(e).uint8(std::to_underlying(error)).uint8(payload);
    }
    template<Decoder D>
    static std::expected<Reply, std::errc> decode(D& d) {
      auto const head{Head::decode(d)};
      if (!head || !d.has_at_least(sizeof(error) + sizeof(payload)))
        return std::unexpected(std::errc::invalid_argument);
      return Reply{.head = *head,
                   .error = static_cast<Error>(d.uint8()),
                   .payload = d.uint8()};
    }
  };

  using Head = detail::Head<Command::AccessoryControl>;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    Sender::CommandStation,
                                    StationType::MX1}};
  bitfields::DecoderAddress cAdr{}; ///< Address
  uint8_t cData{};                  ///< Data
  template<Encoder E>
  E encode(E e) const {
    return head.encode(e).uint16(cAdr).uint8(cData);
  }
  template<Decoder D>
  static std::expected<AccessoryControl, std::errc> decode(D& d) {
    auto const head{Head::decode(d)};
    if (!head || !d.has_at_least(sizeof(cAdr) + sizeof(cData)))
      return std::unexpected(std::errc::invalid_argument);
    return AccessoryControl{
      .head = *head, .cAdr = d.uint16(), .cData = d.uint8()};
  }
};

} // namespace ulf::mx1bin
