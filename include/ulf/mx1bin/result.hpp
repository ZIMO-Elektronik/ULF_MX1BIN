#pragma once

#include <cstdint>
#include <dcc/dcc.hpp>
#include <variant>
#include "result.hpp"

namespace ulf::mx1bin {

struct Result {
  ShortHead head;
  std::variant<dcc::Packet, bool> payload{};
};

} // namespace ulf::mx1bin
