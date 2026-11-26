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
#include "../fields.hpp"
#include "message_base.hpp"

namespace ulf::mx1bin {

struct AddressControl {
  struct Reply {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::L1Ack,
                                      detail::defaultSender,
                                      StationType::MX1},
              .code = Command::AddressControl};
    bitfields::AddressControl_Payload payload{}; ///< Control params
    uint8_t cOutputs{};                          ///< Acc decoder outputs

    using members =
      member_list<member<&Reply::payload>, member<&Reply::cOutputs>>;

    constexpr bool operator==(Reply const&) const = default;
    constexpr Reply& operator=(Reply const&) = default;
  };
  using Head = detail::Head;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    detail::defaultSender,
                                    StationType::MX1},
            .code = Command::AddressControl};
  bitfields::DecoderAddress cAdr{};             ///< Address
  bitfields::AddressControl_Control cControl{}; ///< Control parameters
  std::optional<uint8_t> cOutputs{};            ///< Acc decoder outputs

  using members = member_list<member<&AddressControl::cAdr>,
                              member<&AddressControl::cControl>,
                              member<&AddressControl::cOutputs>>;

  constexpr bool operator==(AddressControl const&) const = default;
  constexpr AddressControl& operator=(AddressControl const&) = default;
};

} // namespace ulf::mx1bin
