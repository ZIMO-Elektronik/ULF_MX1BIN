#include <gtest/gtest.h>

#include "ulf/mx1bin.hpp"

#define CODABLE(type)                                                          \
  static_assert(ulf::mx1bin::Encodable<type>);                                 \
  static_assert(ulf::mx1bin::Decodable<type>);

#define CODE(var)                                                              \
  auto encoded{encode(var)};                                                   \
  auto const decoded{ulf::mx1bin::decode<decltype(var)>(encoded)};             \
  ASSERT_TRUE(decoded) << "Error during decoding";                             \
  auto const d_##var{*decoded};

#define MATCH_HEAD(var)                                                        \
  ASSERT_EQ(var.uSID, d_##var.uSID);                                           \
  ASSERT_EQ(var.info, d_##var.info);                                           \
  ASSERT_EQ(var.code, d_##var.code);

template<ulf::mx1bin::Encodable E>
constexpr ulf::mx1bin::Packet encode(E& e) {
  ulf::mx1bin::Packet result;
  ulf::mx1bin::detail::Encoder en{result};
  en = e.encode(en);
  result.resize(en.difference());
  return result;
}

TEST(Message, Ack) {
  CODABLE(ulf::mx1bin::Ack)

  ulf::mx1bin::Ack ack{{.uSID{0x00},
                        .info{0x01},
                        .code{ulf::mx1bin::Command::Track_Ctrl},
                        .reply_uSID{0x03}}};

  CODE(ack);

  MATCH_HEAD(ack);
  ASSERT_EQ(ack.reply_uSID, d_ack.reply_uSID);
}

TEST(Message, Nak) {
  CODABLE(ulf::mx1bin::Nak)

  ulf::mx1bin::Nak nak{
    {.uSID{0x00}, .info{0x01}, .code{ulf::mx1bin::Command::Track_Ctrl}}};

  CODE(nak)

  MATCH_HEAD(nak)
}

TEST(Message, Reset) {
  CODABLE(ulf::mx1bin::Reset)

  ulf::mx1bin::Reset reset{
    {.uSID{0x00}, .info{0x01}, .code{ulf::mx1bin::Command::Reset}}};

  CODE(reset)

  MATCH_HEAD(reset)
}

TEST(Message, TrackControl) {
  CODABLE(ulf::mx1bin::TrackControl)

  ulf::mx1bin::TrackControl ctrl{
    {.uSID{0x00}, .info{0x01}, .code{ulf::mx1bin::Command::Track_Ctrl}}};
  ctrl.cAction = 0x02;

  CODE(ctrl)

  MATCH_HEAD(ctrl)
  ASSERT_EQ(ctrl.cAction, d_ctrl.cAction);
}

TEST(Message, TrackControl_Reply) {}
TEST(Message, DecoderControl) {}
TEST(Message, DecoderControl_Reply) {
  CODABLE(ulf::mx1bin::DecoderControl::Reply)

  ulf::mx1bin::DecoderControl::Reply reply{
    {{{.uSID{0x00},
       .info{0x01},
       .code{ulf::mx1bin::Command::Track_Ctrl},
       .reply_uSID{0x03}}}}};
  reply.error = ulf::mx1bin::Error::NO_ERROR;
  reply.payload = 0x8Cu;

  CODE(reply);

  MATCH_HEAD(reply);
  ASSERT_EQ(reply.reply_uSID, d_reply.reply_uSID);
  ASSERT_EQ(reply.error, d_reply.error);
  ASSERT_EQ(reply.payload, d_reply.payload);
}

TEST(Message, InvertFunctionBits) {}
TEST(Message, InvertFunctionBits_Reply) {}
TEST(Message, Acceleration) {}
TEST(Message, Acceleration_Reply) {}
TEST(Message, ShuttleTrain) {}
TEST(Message, ShuttleTrain_Reply) {}
TEST(Message, Accessory) {}
TEST(Message, Accessory_Reply) {}
TEST(Message, LocoMemoryQuery) {}
TEST(Message, LocoMemoryQuery_Reply) {}
TEST(Message, AccessoryMemoryQuery) {}
TEST(Message, AccessoryMemoryQuery_Reply) {}
TEST(Message, AddressControl) {}
TEST(Message, AddressControl_Reply) {}
TEST(Message, CommandStationIOQuery) {}
TEST(Message, CommandStationIOQuery_Reply) {}
TEST(Message, CommandStationCvManip) {}
TEST(Message, CommandStationCvManip_Reply) {}
TEST(Message, CommandStationEquipmentQuery) {}
TEST(Message, CommandStationEquipmentQuery_Reply) {}
TEST(Message, SerialInfo) {}
TEST(Message, DecoderCvManip) {}
TEST(Message, DecoderCvManip_Reply) {}
TEST(Message, DecoderCvManip_Busy) {}
TEST(Message, DecoderCvManip_Erro) {}
