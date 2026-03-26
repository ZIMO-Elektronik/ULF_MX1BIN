#pragma once

#include <cstdint>
#include "../bitfields.hpp"
#include "../commands.hpp"
#include "../decoder.hpp"
#include "../encoder.hpp"
#include "../error.hpp"
#include "message_base.hpp"

namespace ulf::mx1bin {

struct Reset {
  using Head = detail::Head<Command::Reset>;
  Head head{.info = bitfields::Info{FrameType::Short,
                                    MessageType::L1Ack,
                                    Sender::CommandStation,
                                    StationType::MX1}};
  template<Encoder E>
  E encode(E e) const {
    return head.encode(e);
  }
  template<Decoder D>
  static std::expected<Reset, std::errc> decode(D& d) {
    if (auto const head{Head::decode(d)}) return Reset{.head = *head};
    else return std::unexpected(head.error());
  }
};

} // namespace ulf::mx1bin
