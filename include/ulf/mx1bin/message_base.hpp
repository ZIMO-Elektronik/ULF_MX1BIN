// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Message Base structs
///
/// \file   ulf/mx1bin/message_base.hpp
/// \author Jonas Gahlert
/// \date   16/07/2025

#pragma once

#include <cstdint>
#include <span>
#include <ztl/inplace_vector.hpp>
#include "utility.hpp"

namespace ulf::mx1bin::detail {

struct Head {
  uint8_t uSID{};
  uint8_t type{};
  uint8_t code{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt& out) {
    detail::encode_8(uSID, out);
    detail::encode_8(type, out);
    detail::encode_8(code, out);
    return out;
  }
  template<std::input_iterator InputIt>
  Head& decode(InputIt& in) {
    uSID = detail::decode_8(in);
    type = detail::decode_8(in);
    code = detail::decode_8(in);
    return *this;
  }
  Head& decode(std::span<uint8_t const> bytes) {
    auto iter{begin(bytes)};
    return decode(iter);
  }
};

struct DecoderControlBase : public Head {
  uint16_t cAdr{};
  template<std::input_iterator InputIt>
  DecoderControlBase& decode(InputIt& in) {
    Head::decode(in);
    cAdr = detail::decode_16(in);
    return *this;
  }
  DecoderControlBase& decode(std::span<uint8_t const> bytes) {
    auto iter{begin(bytes)};
    return decode(iter);
  }
};

struct ShuttleTrain_Accessory_Base : public DecoderControlBase {
  uint8_t cData{};
  template<std::input_iterator InputIt>
  ShuttleTrain_Accessory_Base& decode(InputIt& in) {
    DecoderControlBase::decode(in);
    cData = detail::decode_8(in);
    return *this;
  }
  ShuttleTrain_Accessory_Base& decode(std::span<uint8_t const> bytes) {
    auto iter{begin(bytes)};
    return decode(iter);
  }
};

struct CommandStationQueryBase : public detail::Head {
  uint8_t zero;
  template<std::input_iterator InputIt>
  CommandStationQueryBase& decode(InputIt& in) {
    Head::decode(in);
    zero = detail::decode_8(in);
    return *this;
  }
  CommandStationQueryBase& decode(std::span<uint8_t const> bytes) {
    auto iter{begin(bytes)};
    return decode(iter);
  }
};

struct ReplyHead {
  uint8_t uSID{};
  uint8_t type{};
  uint8_t code{};
  uint8_t reply_uSID{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt& out) {
    detail::encode_8(uSID, out);
    detail::encode_8(type, out);
    detail::encode_8(code, out);
    detail::encode_8(reply_uSID, out);
    return out;
  }
  template<std::input_iterator InputIt>
  ReplyHead& decode(InputIt& in) {
    uSID = detail::decode_8(in);
    type = detail::decode_8(in);
    code = detail::decode_8(in);
    reply_uSID = detail::decode_8(in);
    return *this;
  }
  ReplyHead& decode(std::span<uint8_t const> bytes) {
    auto iter{begin(bytes)};
    return decode(iter);
  }
};

struct ReplyLongHead {
  uint8_t uSID{};
  uint8_t type{};
  uint8_t code{};
  uint8_t lengthOfHeader{};
  uint8_t reply_uSID{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt& out) {
    detail::encode_8(uSID, out);
    detail::encode_8(type, out);
    detail::encode_8(code, out);
    detail::encode_8(lengthOfHeader, out);
    detail::encode_8(reply_uSID, out);
    return out;
  }
};

struct ReplyErrorBase : public ReplyHead {
  uint8_t error{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt& out) {
    ReplyHead::encode(out);
    detail::encode_8(error, out);
    return out;
  }
};

struct ReplyDecoderControlBase : public ReplyErrorBase {
  uint8_t payload{};
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt& out) {
    ReplyErrorBase::encode(out);
    detail::encode_8(payload, out);
    return out;
  }
};

} // namespace ulf::mx1bin::detail
