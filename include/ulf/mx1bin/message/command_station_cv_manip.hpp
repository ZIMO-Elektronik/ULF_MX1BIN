// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Command Station CV Manip Message
///
/// \file   ulf/mx1bin/message/command_station_cv_manip.hpp
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

struct CommandStationCvManip {
  struct Reply {
    using Head = detail::ReplyHead;
    Head head{.info = bitfields::Info{FrameType::Short,
                                      MessageType::L1Ack,
                                      detail::defaultSender,
                                      StationType::MX1},
              .code = Command::CommandStationCvManip};
    Error error{Error::NO_ERROR}; ///< Error
    uint8_t value{};              ///< CV Value
    template<Encoder E>
    E encode(E e) const {
      return head.encode(e).uint8(std::to_underlying(error)).uint8(value);
    }
    template<Decoder D>
    static std::expected<Reply, std::errc> decode(D& d) {
      auto const head{Head::decode(d)};
      if (!head || !d.has_at_least(sizeof(error) + sizeof(value)))
        return std::unexpected{std::errc::invalid_argument};
      return Reply{.head = *head,
                   .error = static_cast<Error>(d.uint8()),
                   .value = d.uint8()};
    }
    constexpr bool operator==(Reply const&) const = default;
    constexpr Reply& operator=(Reply const&) = default;
  };
  using Head = detail::Head;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    detail::defaultSender,
                                    StationType::MX1},
            .code = Command::CommandStationCvManip};
  uint16_t variable{};            ///< Cv Address
  std::optional<uint8_t> value{}; ///< Cv Value
  template<Encoder E>
  E encode(E e) const {
    return head.encode(e).uint16(variable).uint8(value);
  }
  template<Decoder D>
  static std::expected<CommandStationCvManip, std::errc> decode(D& d) {
    auto const head{Head::decode(d)};
    if (!head || !d.has_at_least(sizeof(variable)))
      return std::unexpected(std::errc::invalid_argument);
    return CommandStationCvManip{.head = *head,
                                 .variable = d.uint16(),
                                 // Add optional data
                                 .value = d.s_uint8()};
  }
  constexpr bool operator==(CommandStationCvManip const&) const = default;
  constexpr CommandStationCvManip&
  operator=(CommandStationCvManip const&) = default;
};

} // namespace ulf::mx1bin
