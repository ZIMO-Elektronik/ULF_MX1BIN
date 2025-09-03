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
#include "commands.hpp"
#include "decoder.hpp"
#include "encoder.hpp"
#include "utility.hpp"

namespace ulf::mx1bin::detail {

struct Head {
  uint8_t uSID{}; ///> Unique Service ID
  uint8_t info{}; ///> Header info byte
  Command code{}; ///> Command code
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt& out) const {
    detail::encode_8(uSID, out);
    detail::encode_8(info, out);
    detail::encode_8(std::to_underlying(code), out);
    return out;
  }
  template<decoder D>
  static std::expected<Head, std::errc> decode(D& d) {
    if (!d.has_at_least(sizeof(uSID) + sizeof(info) + sizeof(code)))
      return std::unexpected(std::errc::invalid_argument);
    return Head{.uSID{d.uint8()}, .info{d.uint8()}, .code{d.uint8()}};
  }
};

struct DecoderControlBase : public Head {
  uint16_t cAdr{}; ///> Decoder Address
  template<decoder D>
  static std::expected<DecoderControlBase, std::errc> decode(D& d) {
    auto const base{Head::decode(d)};
    if (!base || !d.has_at_least(sizeof(cAdr)))
      return std::unexpected(std::errc::invalid_argument);
    DecoderControlBase result{*base};
    result.cAdr = d.uint16();
    return result;
  }
};

struct ShuttleTrain_Accessory_Base : public DecoderControlBase {
  uint8_t cData{}; ///>
  template<decoder D>
  static std::expected<ShuttleTrain_Accessory_Base, std::errc> decode(D& d) {
    auto const base{DecoderControlBase::decode(d)};
    if (!base || !d.has_at_least(sizeof(cData)))
      return std::unexpected(std::errc::invalid_argument);
    ShuttleTrain_Accessory_Base result{*base};
    result.cData = d.uint8();
    return result;
  }
};

struct CommandStationQueryBase : public detail::Head {
  uint8_t zero{}; ///>
  template<decoder D>
  static std::expected<CommandStationQueryBase, std::errc> decode(D& d) {
    auto const base{Head::decode(d)};
    if (!base || !d.has_at_least(sizeof(zero)))
      return std::unexpected(std::errc::invalid_argument);
    CommandStationQueryBase result{*base};
    result.zero = d.uint8();
    return result;
  }
};

struct ReplyHead {
  uint8_t uSID{};       ///> Unique Service ID
  uint8_t info{};       ///> Header info byte
  Command code{};       ///> Command code
  uint8_t reply_uSID{}; ///> Reply uSID reference
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt& out) const {
    detail::encode_8(uSID, out);
    detail::encode_8(info, out);
    detail::encode_8(std::to_underlying(code), out);
    detail::encode_8(reply_uSID, out);
    return out;
  }
  template<decoder D>
  static std::expected<ReplyHead, std::errc> decode(D& d) {
    if (!d.has_at_least(sizeof(uSID) + sizeof(info) + sizeof(Command) +
                        sizeof(reply_uSID)))
      return std::unexpected(std::errc::invalid_argument);
    return ReplyHead{.uSID = d.uint8(),
                     .info = d.uint8(),
                     .code = static_cast<Command>(d.uint8()),
                     .reply_uSID = d.uint8()};
  }
};

struct ReplyLongHead {
  uint8_t uSID{};           ///> Unique Service ID
  uint8_t info{};           ///> Header info byte
  Command code{};           ///> Command code
  uint8_t lengthOfHeader{}; ///> Length of header
  uint8_t reply_uSID{};     ///> Reply uSID reference
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt& out) const {
    detail::encode_8(uSID, out);
    detail::encode_8(info, out);
    detail::encode_8(std::to_underlying(code), out);
    detail::encode_8(lengthOfHeader, out);
    detail::encode_8(reply_uSID, out);
    return out;
  }
};

struct ReplyErrorBase : public ReplyHead {
  Error error{}; ///> Error
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt& out) const {
    ReplyHead::encode(out);
    detail::encode_8(std::to_underlying(error), out);
    return out;
  }
};

struct ReplyDecoderControlBase : public ReplyErrorBase {
  uint8_t payload{}; ///> Payload
  template<std::output_iterator<uint8_t> OutputIt>
  auto encode(OutputIt& out) const {
    ReplyErrorBase::encode(out);
    detail::encode_8(payload, out);
    return out;
  }
};

} // namespace ulf::mx1bin::detail
