#include <gtest/gtest.h>
#include <ulf/mx1bin.hpp>
#include "helper.hpp"

#define CODABLE(type)                                                          \
  static_assert(ulf::mx1bin::Encodable<type>);                                 \
  static_assert(ulf::mx1bin::Decodable<type>);

#define CODE(var)                                                              \
  auto encoded{mencode(var)};                                                  \
  auto const decoded{ulf::mx1bin::decode<decltype(var)>(encoded)};             \
  ASSERT_TRUE(decoded) << "Error during decoding";                             \
  auto const d_##var{*decoded};

#define MATCH_HEAD(var)                                                        \
  ASSERT_EQ(var.head.uSID, d_##var.head.uSID);                                 \
  ASSERT_EQ(var.head.info.frameType, d_##var.head.info.frameType);             \
  ASSERT_EQ(var.head.info.messageType, d_##var.head.info.messageType);         \
  ASSERT_EQ(var.head.info.sender, d_##var.head.info.sender);                   \
  ASSERT_EQ(var.head.info.stationType, d_##var.head.info.stationType);         \
  ASSERT_EQ(var.head.code, d_##var.head.code);

template<ulf::mx1bin::Encodable E>
constexpr ulf::mx1bin::Packet mencode(E& e) {
  ulf::mx1bin::Packet result;
  ulf::mx1bin::StreamEncoder en{result};
  en = e.encode(en);
  return result;
}

TEST(Message, Ack) {
  static_assert(Codable<ulf::mx1bin::Ack>);

  ulf::mx1bin::Ack message{};

  message.head.reply_uSID = 0x03u;

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
}

TEST(Message, Nak) {
  static_assert(Codable<ulf::mx1bin::Nak>);

  ulf::mx1bin::Nak message{};

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
}

TEST(Message, Reset) {
  static_assert(Codable<ulf::mx1bin::Reset>);

  ulf::mx1bin::Reset message{};

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
}

TEST(Message, TrackControl) {
  static_assert(Codable<ulf::mx1bin::TrackControl>);

  ulf::mx1bin::TrackControl message{.cAction =
                                      decltype(message)::Action::TrackOn};

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.cAction, result.message.cAction);
}

TEST(Message, TrackControl_Reply) {
  static_assert(Codable<ulf::mx1bin::TrackControl::Reply>);

  ulf::mx1bin::TrackControl::Reply message{.statusBits = 0x55u};
  message.head.reply_uSID = 0x03u;

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.statusBits, result.message.statusBits);
}

TEST(Message, LocoControl) {
  static_assert(Codable<ulf::mx1bin::LocoControl>);

  ulf::mx1bin::LocoControl message{.cAdr = 0x8003u,
                                   .cSpeed = 0xAAu,
                                   .cData1 = 0x55u,
                                   .cData2 = 0xAAu,
                                   .cData3 = 0x55u};

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.cSpeed, result.message.cSpeed);
  ASSERT_EQ(message.cData1, result.message.cData1);
  ASSERT_EQ(message.cData2, result.message.cData2);
  ASSERT_EQ(message.cData3, result.message.cData3);
  ASSERT_FALSE(result.message.cData4);
  ASSERT_FALSE(result.message.cData5);
}

TEST(Message, LocoControl_Reply) {
  static_assert(Codable<ulf::mx1bin::LocoControl::Reply>);

  ulf::mx1bin::LocoControl::Reply message{.error = ulf::mx1bin::Error::NO_ERROR,
                                          .payload = 0x8Cu};
  message.head.reply_uSID = 0x03u;

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.error, result.message.error);
  ASSERT_EQ(message.payload, result.message.payload);
}

TEST(Message, InvertFunctionBits) {
  static_assert(Codable<ulf::mx1bin::InvertFunctionBits>);

  ulf::mx1bin::InvertFunctionBits message{.cAdr = 0x8003u,
                                          .cData1 = 0x55u,
                                          .cData2 = 0xAAu,
                                          .cData3 = 0x55u,
                                          .cData4 = 0xAAu,
                                          .cData5 = 0x55u};

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.cAdr, result.message.cAdr);
  ASSERT_EQ(message.cData1, result.message.cData1);
  ASSERT_EQ(message.cData2, result.message.cData2);
  ASSERT_EQ(message.cData3, result.message.cData3);
  ASSERT_EQ(message.cData4, result.message.cData4);
  ASSERT_EQ(message.cData5, result.message.cData5);
}

TEST(Message, InvertFunctionBits_Reply) {
  static_assert(Codable<ulf::mx1bin::InvertFunctionBits::Reply>);

  ulf::mx1bin::InvertFunctionBits::Reply message{
    .error = ulf::mx1bin::Error::NO_ERROR, .payload = 0x8Cu};
  message.head.reply_uSID = 0x03u;

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.error, result.message.error);
  ASSERT_EQ(message.payload, result.message.payload);
}

TEST(Message, Acceleration) {
  static_assert(Codable<ulf::mx1bin::Acceleration>);

  ulf::mx1bin::Acceleration message{.cAdr = 0x8003u, .cAzBz = 0x55u};

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.cAdr, result.message.cAdr);
  ASSERT_EQ(message.cAzBz, result.message.cAzBz);
}

TEST(Message, Acceleration_Reply) {
  static_assert(Codable<ulf::mx1bin::Acceleration::Reply>);

  ulf::mx1bin::Acceleration::Reply message{
    .error = ulf::mx1bin::Error::NO_ERROR, .payload = 0x8Cu};
  message.head.reply_uSID = 0x03u;

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.error, result.message.error);
  ASSERT_EQ(message.payload, result.message.payload);
}

TEST(Message, ShuttleTrain) {
  static_assert(Codable<ulf::mx1bin::ShuttleTrain>);

  ulf::mx1bin::ShuttleTrain message{.cAdr = 0x8003u, .cData = 0x55u};

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.cAdr, result.message.cAdr);
  ASSERT_EQ(message.cData, result.message.cData);
}

TEST(Message, ShuttleTrain_Reply) {
  static_assert(Codable<ulf::mx1bin::ShuttleTrain::Reply>);

  ulf::mx1bin::ShuttleTrain::Reply message{
    reply.error = ulf::mx1bin::Error::NO_ERROR, reply.payload = 0x8Cu};
  reply.head.reply_uSID = 0x03u;

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.error, result.message.error);
  ASSERT_EQ(message.payload, result.message.payload);
}

TEST(Message, AccessoryControl) {
  static_assert(Codable<ulf::mx1bin::AccessoryControl>);

  ulf::mx1bin::AccessoryControl message{msg.cAdr = 0x8003u, msg.cData = 0x55u};

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.cAdr, result.message.cAdr);
  ASSERT_EQ(message.cData, result.message.cData);
}

TEST(Message, AccessoryControl_Reply) {
  static_assert(Codable<ulf::mx1bin::AccessoryControl::Reply>);

  ulf::mx1bin::AccessoryControl::Reply message{
    .error = ulf::mx1bin::Error::NO_ERROR, .payload = 0x8Cu};
  reply.head.reply_uSID = 0x03u;

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.error, result.message.error);
  ASSERT_EQ(message.payload, result.message.payload);
}

TEST(Message, LocoMemoryQuery) {
  static_assert(Codable<ulf::mx1bin::LocoMemoryQuery>);

  ulf::mx1bin::LocoMemoryQuery message{.cAdr = 0x8003u};

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.cAdr, message.cAdr);
}

TEST(Message, LocoMemoryQuery_Reply) {
  CODABLE(ulf::mx1bin::LocoMemoryQuery::Reply)

  ulf::mx1bin::LocoMemoryQuery::Reply reply{};
  reply.head.reply_uSID = 0x03u;
  reply.error = ulf::mx1bin::Error::NO_ERROR;
  reply.cAdr = 0x8003u;
  reply.cSpeed = 0x02u;
  reply.cData1 = 0x05u;
  reply.cData2 = 0x06u;
  reply.cData3 = 0x07u;
  reply.cAzBz = 0x00u;
  reply.cStatus = 0x01u;
  reply.cData4 = 0x08u;
  reply.cData5 = 0x09u;

  CODE(reply)

  MATCH_HEAD(reply)
  ASSERT_EQ(reply.head.reply_uSID, d_reply.head.reply_uSID);
  ASSERT_EQ(reply.error, d_reply.error);
  ASSERT_EQ(reply.cAdr, d_reply.cAdr);
  ASSERT_EQ(reply.cSpeed, d_reply.cSpeed);
  ASSERT_EQ(reply.cData1, d_reply.cData1);
  ASSERT_EQ(reply.cData2, d_reply.cData2);
  ASSERT_EQ(reply.cData3, d_reply.cData3);
  ASSERT_EQ(reply.cAzBz, d_reply.cAzBz);
  ASSERT_EQ(reply.cStatus, d_reply.cStatus);
  ASSERT_EQ(reply.cData4, d_reply.cData4);
  ASSERT_EQ(reply.cData5, d_reply.cData5);
}

TEST(Message, AccessoryMemoryQuery) {
  CODABLE(ulf::mx1bin::AccessoryMemoryQuery)

  ulf::mx1bin::AccessoryMemoryQuery msg{};

  msg.cAdr = 0x8003u;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.cAdr, d_msg.cAdr);
}

TEST(Message, AccessoryMemoryQuery_Reply) {
  CODABLE(ulf::mx1bin::AccessoryMemoryQuery::Reply)

  ulf::mx1bin::AccessoryMemoryQuery::Reply msg{};
  msg.head.reply_uSID = 0x03u;

  msg.error = ulf::mx1bin::Error::NO_ERROR;
  msg.cAdr = 0x8003u;
  msg.cPair = 0x55u;
  msg.cOutputs = 0xAAu;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.head.reply_uSID, d_msg.head.reply_uSID);
  ASSERT_EQ(msg.error, d_msg.error);
  ASSERT_EQ(msg.cAdr, d_msg.cAdr);
  ASSERT_EQ(msg.cPair, d_msg.cPair);
  ASSERT_EQ(msg.cOutputs, d_msg.cOutputs);
}

TEST(Message, AddressControl) {
  CODABLE(ulf::mx1bin::AddressControl)

  ulf::mx1bin::AddressControl msg{};

  msg.cAdr = 0x8003u;
  msg.cControl = 0x55u;
  msg.cOutputs = 0xAAu;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.cAdr, d_msg.cAdr);
  ASSERT_EQ(msg.cControl, d_msg.cControl);
  ASSERT_EQ(msg.cOutputs, d_msg.cOutputs);
}

TEST(Message, AddressControl_Reply) {
  CODABLE(ulf::mx1bin::AddressControl::Reply)

  ulf::mx1bin::AddressControl::Reply msg{};
  msg.head.reply_uSID = 0x03u;

  msg.payload = 0x55u;
  msg.cOutputs = 0xAAu;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.head.reply_uSID, d_msg.head.reply_uSID);
  ASSERT_EQ(msg.payload, d_msg.payload);
  ASSERT_EQ(msg.cOutputs, d_msg.cOutputs);
}

TEST(Message, CommandStationIOQuery) {
  CODABLE(ulf::mx1bin::CommandStationIOQuery)

  ulf::mx1bin::CommandStationIOQuery msg{};

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.zero, d_msg.zero);
}

TEST(Message, CommandStationIOQuery_Reply) {
  CODABLE(ulf::mx1bin::CommandStationIOQuery::Reply)

  ulf::mx1bin::CommandStationIOQuery::Reply msg{};
  msg.head.reply_uSID = 0x03u;

  msg.cCurrent1 = 0x55u;
  msg.cVoltage1 = 0xAAu;
  msg.cCurrent2 = 0x55u;
  msg.cVoltage2 = 0xAAu;
  msg.cAux = 0x55u;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.head.reply_uSID, d_msg.head.reply_uSID);
  ASSERT_EQ(msg.cCurrent1, d_msg.cCurrent1);
  ASSERT_EQ(msg.cVoltage1, d_msg.cVoltage1);
  ASSERT_EQ(msg.cCurrent2, d_msg.cCurrent2);
  ASSERT_EQ(msg.cVoltage2, d_msg.cVoltage2);
  ASSERT_EQ(msg.cAux, d_msg.cAux);
}

TEST(Message, CommandStationCvManip) {
  CODABLE(ulf::mx1bin::CommandStationCvManip)

  ulf::mx1bin::CommandStationCvManip msg{};

  msg.variable = 0x55u;
  msg.value = 0xAAu;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.variable, d_msg.variable);
  ASSERT_EQ(*msg.value, *d_msg.value);
}

TEST(Message, CommandStationCvManip_Reply) {
  CODABLE(ulf::mx1bin::CommandStationCvManip::Reply)

  ulf::mx1bin::CommandStationCvManip::Reply msg{};
  msg.head.reply_uSID = 0x03u;

  msg.error = ulf::mx1bin::Error::NO_ERROR;
  msg.value = 0x55u;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.head.reply_uSID, d_msg.head.reply_uSID);
  ASSERT_EQ(msg.error, d_msg.error);
  ASSERT_EQ(msg.value, d_msg.value);
}

TEST(Message, CommandStationEquipmentQuery) {
  CODABLE(ulf::mx1bin::CommandStationEquipmentQuery)

  ulf::mx1bin::CommandStationEquipmentQuery msg{};

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.zero, d_msg.zero);
}

// TEST(Message, CommandStationEquipmentQuery_Reply) {}

TEST(Message, SerialInfo) {
  CODABLE(ulf::mx1bin::SerialInfo)

  ulf::mx1bin::SerialInfo msg{{.uSID = 0x00u, .info = 0x01u}};

  msg.action = 0x55u;
  msg.toolID = 0xAAu;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.action, d_msg.action);
  ASSERT_EQ(msg.toolID, d_msg.toolID);
}

TEST(Message, DecoderCvManip) {
  CODABLE(ulf::mx1bin::DecoderCvManip)

  ulf::mx1bin::DecoderCvManip msg{};

  msg.cAdr = 0x8003u;
  msg.variable = 0x55u;
  msg.value = 0xAAu;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.cAdr, d_msg.cAdr);
  ASSERT_EQ(msg.variable, d_msg.variable);
  ASSERT_EQ(*msg.value, *d_msg.value);
}

// TEST(Message, DecoderCvManip_Reply) {}

// TEST(Message, DecoderCvManip_Busy) {}

// TEST(Message, DecoderCvManip_Erro) {}
