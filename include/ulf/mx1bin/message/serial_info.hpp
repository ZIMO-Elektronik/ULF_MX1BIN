#pragma once

#include <cstdint>
#include "../bitfields.hpp"
#include "../commands.hpp"
#include "../decoder.hpp"
#include "../encoder.hpp"
#include "../error.hpp"
#include "message_base.hpp"

namespace ulf::mx1bin {

struct SerialInfo {
  using Head = detail::Head<Command::SerialInfo>;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::Primary,
                                    Sender::CommandStation,
                                    StationType::MX1}};
  uint8_t toolID{}; ///< Tool ID
  uint8_t action{}; ///<
  template<Encoder E>
  E encode(E e) const {
    return head.encode(e).uint8(toolID).uint8(action);
  }
  template<Decoder D>
  static std::expected<SerialInfo, std::errc> decode(D& d) {
    auto const head{Head::decode(d)};
    if (!head || !d.has_at_least(sizeof(toolID) + sizeof(action)))
      return std::unexpected(std::errc::invalid_argument);
    return SerialInfo{.head = *head, .toolID = d.uint8(), .action = d.uint8()};
  }
};

} // namespace ulf::mx1bin
