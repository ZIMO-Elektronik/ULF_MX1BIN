// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Address Control Message
///
/// \file   ulf/mx1bin/message/address_control.hpp
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

struct AddressControl {
  struct Reply {
    using Head = detail::ReplyHead<Command::AddressControl>;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::L1Ack,
                                      Sender::CommandStation,
                                      StationType::MX1}};
    bitfields::AddressControl_Payload payload{}; ///< Control params
    uint8_t cOutputs{};                          ///< Acc decoder outputs
    template<Encoder E>
    E encode(E e) const {
      return head.encode(e).uint8(payload).uint8(cOutputs);
    }
    template<Decoder D>
    static std::expected<Reply, std::errc> decode(D& d) {
      auto const head{Head::decode(d)};
      if (!head || !d.has_at_least(sizeof(payload) + sizeof(cOutputs)))
        return std::unexpected{std::errc::invalid_argument};
      return Reply{.head = *head, .payload = d.uint8(), .cOutputs = d.uint8()};
    }
  };
  using Head = detail::Head<Command::AddressControl>;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    Sender::CommandStation,
                                    StationType::MX1}};
  bitfields::DecoderAddress cAdr{};             ///< Address
  bitfields::AddressControl_Control cControl{}; ///< Control parameters
  std::optional<uint8_t> cOutputs{};            ///< Acc decoder outputs
  template<Encoder E>
  E encode(E e) const {
    return head.encode(e).uint16(cAdr).uint8(cControl).uint8(cOutputs);
  }
  template<Decoder D>
  static std::expected<AddressControl, std::errc> decode(D& d) {
    auto const head{Head::decode(d)};
    if (!head || !d.has_at_least(sizeof(cAdr) + sizeof(cControl)))
      return std::unexpected(std::errc::invalid_argument);
    return AddressControl{.head = *head,
                          .cAdr = d.uint16(),
                          .cControl = d.uint8(),
                          // Add optional data
                          .cOutputs = d.s_uint8()};
  }
};

} // namespace ulf::mx1bin
