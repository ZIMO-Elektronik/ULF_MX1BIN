// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Loco Control Message
///
/// \file   ulf/mx1bin/message/loco_control.hpp
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

struct LocoControl {
  struct Reply {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::L1Ack,
                                      Sender::CommandStation,
                                      StationType::MX1},
              .code = Command::LocoControl};
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
  using Head = detail::Head;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    Sender::CommandStation,
                                    StationType::MX1},
            .code = Command::LocoControl};
  bitfields::DecoderAddress cAdr{};               ///< Address
  bitfields::ControlSpeed cSpeed{};               ///< Speed
  std::optional<bitfields::ControlData> cData1{}; ///< Control data
  std::optional<uint8_t> cData2{};                ///< F1..8
  std::optional<uint8_t> cData3{};                ///< F9..12
  std::optional<uint8_t> cData4{};                ///< F13..20
  std::optional<uint8_t> cData5{};                ///< F21..28
  template<Encoder E>
  E encode(E e) const {
    return head.encode(e)
      .uint16(cAdr)
      .uint8(cSpeed)
      .uint8(cData1)
      .uint8(cData2)
      .uint8(cData3)
      .uint8(cData4)
      .uint8(cData5);
  }
  template<Decoder D>
  static std::expected<LocoControl, std::errc> decode(D& d) {
    auto const head{Head::decode(d)};
    if (!head || !d.has_at_least(sizeof(cAdr) + sizeof(cSpeed)))
      return std::unexpected(std::errc::invalid_argument);
    return LocoControl{.head = *head,
                       .cAdr = d.uint16(),
                       .cSpeed = d.uint8(),
                       // Add optional data
                       .cData1 = d.s_uint8(),
                       .cData2 = d.s_uint8(),
                       .cData3 = d.s_uint8(),
                       .cData4 = d.s_uint8(),
                       .cData5 = d.s_uint8()};
  }
};

} // namespace ulf::mx1bin
