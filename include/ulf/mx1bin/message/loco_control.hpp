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
#include "../fields.hpp"
#include "message_base.hpp"

namespace ulf::mx1bin {

struct LocoControl {
  struct Reply {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::L1Ack,
                                      detail::defaultSender,
                                      StationType::MX1},
              .code = Command::LocoControl};
    Error error{Error::NO_ERROR};
    bitfields::ControlPayload payload{};

    using members = member_list<member<&Reply::error>, member<&Reply::payload>>;

    constexpr bool operator==(Reply const&) const = default;
    constexpr Reply& operator=(Reply const&) = default;
  };
  using Head = detail::Head;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    detail::defaultSender,
                                    StationType::MX1},
            .code = Command::LocoControl};
  bitfields::DecoderAddress cAdr{};               ///< Address
  bitfields::ControlSpeed cSpeed{};               ///< Speed
  std::optional<bitfields::ControlData> cData1{}; ///< Control data
  std::optional<uint8_t> cData2{};                ///< F1..8
  std::optional<uint8_t> cData3{};                ///< F9..12
  std::optional<uint8_t> cData4{};                ///< F13..20
  std::optional<uint8_t> cData5{};                ///< F21..28

  using members = member_list<member<&LocoControl::cAdr>,
                              member<&LocoControl::cSpeed>,
                              member<&LocoControl::cData1>,
                              member<&LocoControl::cData2>,
                              member<&LocoControl::cData3>,
                              member<&LocoControl::cData4>,
                              member<&LocoControl::cData5>>;

  constexpr size_t stream_size() {
    return Head::members::stream_size + members::stream_size;
  }

  constexpr bool operator==(LocoControl const&) const = default;
  constexpr LocoControl& operator=(LocoControl const&) = default;
};

} // namespace ulf::mx1bin
