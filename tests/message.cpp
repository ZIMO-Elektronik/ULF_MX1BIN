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
    .error = ulf::mx1bin::Error::NO_ERROR, .payload = 0x8Cu};
  message.head.reply_uSID = 0x03u;

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.error, result.message.error);
  ASSERT_EQ(message.payload, result.message.payload);
}

TEST(Message, AccessoryControl) {
  static_assert(Codable<ulf::mx1bin::AccessoryControl>);

  ulf::mx1bin::AccessoryControl message{.cAdr = 0x8003u, .cData = 0x55u};

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.cAdr, result.message.cAdr);
  ASSERT_EQ(message.cData, result.message.cData);
}

TEST(Message, AccessoryControl_Reply) {
  static_assert(Codable<ulf::mx1bin::AccessoryControl::Reply>);

  ulf::mx1bin::AccessoryControl::Reply message{
    .error = ulf::mx1bin::Error::NO_ERROR, .payload = 0x8Cu};
  message.head.reply_uSID = 0x03u;

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
  static_assert(Codable<ulf::mx1bin::LocoMemoryQuery::Reply>);

  ulf::mx1bin::LocoMemoryQuery::Reply message{.cAdr = 0x8003u,
                                              .cSpeed = 0x02u,
                                              .cData1 = 0x05u,
                                              .cData2 = 0x06u,
                                              .cData3 = 0x07u,
                                              .cAzBz = 0x00u,
                                              .cStatus = 0x01u,
                                              .cData4 = 0x08u,
                                              .cData5 = 0x09u};
  message.head.reply_uSID = 0x03u;

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);

  ASSERT_EQ(message.error, result.message.error);
  ASSERT_EQ(message.cAdr, result.message.cAdr);
  ASSERT_EQ(message.cSpeed, result.message.cSpeed);
  ASSERT_EQ(message.cData1, result.message.cData1);
  ASSERT_EQ(message.cData2, result.message.cData2);
  ASSERT_EQ(message.cData3, result.message.cData3);
  ASSERT_EQ(message.cAzBz, result.message.cAzBz);
  ASSERT_EQ(message.cStatus, result.message.cStatus);
  ASSERT_EQ(message.cData4, result.message.cData4);
  ASSERT_EQ(message.cData5, result.message.cData5);
}

TEST(Message, AccessoryMemoryQuery) {
  static_assert(Codable<ulf::mx1bin::AccessoryMemoryQuery>);

  ulf::mx1bin::AccessoryMemoryQuery message{.cAdr = 0x8003u};

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.cAdr, result.message.cAdr);
}

TEST(Message, AccessoryMemoryQuery_Reply) {
  static_assert(Codable<ulf::mx1bin::AccessoryMemoryQuery::Reply>);

  ulf::mx1bin::AccessoryMemoryQuery::Reply message{
    .error = ulf::mx1bin::Error::NO_ERROR,
    .cAdr = 0x8003u,
    .cPair = 0x55u,
    .cOutputs = 0xAAu};
  message.head.reply_uSID = 0x03u;

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.head.reply_uSID, result.message.head.reply_uSID);
  ASSERT_EQ(message.error, result.message.error);
  ASSERT_EQ(message.cAdr, result.message.cAdr);
  ASSERT_EQ(message.cPair, result.message.cPair);
  ASSERT_EQ(message.cOutputs, result.message.cOutputs);
}

TEST(Message, AddressControl) {
  static_assert(Codable<ulf::mx1bin::AddressControl>);

  ulf::mx1bin::AddressControl message{
    .cAdr = 0x8003u, .cControl = 0x55u, .cOutputs = 0xAAu};

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.cAdr, result.message.cAdr);
  ASSERT_EQ(message.cControl, result.message.cControl);
  ASSERT_EQ(message.cOutputs, result.message.cOutputs);
}

TEST(Message, AddressControl_Reply) {
  static_assert(Codable<ulf::mx1bin::AddressControl::Reply>);

  ulf::mx1bin::AddressControl::Reply message{.payload = 0x55u,
                                             .cOutputs = 0xAAu};
  message.head.reply_uSID = 0x03u;

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.payload, result.message.payload);
  ASSERT_EQ(message.cOutputs, result.message.cOutputs);
}

TEST(Message, CommandStationIOQuery) {
  static_assert(Codable<ulf::mx1bin::CommandStationIOQuery>);

  ulf::mx1bin::CommandStationIOQuery message{};

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.zero, result.message.zero);
}

TEST(Message, CommandStationIOQuery_Reply) {
  static_assert(Codable<ulf::mx1bin::CommandStationIOQuery::Reply>);

  ulf::mx1bin::CommandStationIOQuery::Reply message{.cCurrent1 = 0x55u,
                                                    .cVoltage1 = 0xAAu,
                                                    .cCurrent2 = 0x55u,
                                                    .cVoltage2 = 0xAAu,
                                                    .cAux = 0x55u};
  message.head.reply_uSID = 0x03u;

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.cCurrent1, result.message.cCurrent1);
  ASSERT_EQ(message.cVoltage1, result.message.cVoltage1);
  ASSERT_EQ(message.cCurrent2, result.message.cCurrent2);
  ASSERT_EQ(message.cVoltage2, result.message.cVoltage2);
  ASSERT_EQ(message.cAux, result.message.cAux);
}

TEST(Message, CommandStationCvManip) {
  static_assert(Codable<ulf::mx1bin::CommandStationCvManip>);

  ulf::mx1bin::CommandStationCvManip message{.variable = 0x55u, .value = 0xAAu};

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.variable, result.message.variable);
  ASSERT_EQ(*message.value, *result.message.value);
}

TEST(Message, CommandStationCvManip_Reply) {
  static_assert(Codable<ulf::mx1bin::CommandStationCvManip::Reply>);

  ulf::mx1bin::CommandStationCvManip::Reply message{

    .error = ulf::mx1bin::Error::NO_ERROR, .value = 0x55u};
  message.head.reply_uSID = 0x03u;

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.error, result.message.error);
  ASSERT_EQ(message.value, result.message.value);
}

TEST(Message, CommandStationEquipmentQuery) {
  static_assert(Codable<ulf::mx1bin::CommandStationEquipmentQuery>);

  ulf::mx1bin::CommandStationEquipmentQuery message{};

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.zero, result.message.zero);
}

// TEST(Message, CommandStationEquipmentQuery_Reply) {}

TEST(Message, SerialInfo) {
  static_assert(Codable<ulf::mx1bin::SerialInfo>);

  ulf::mx1bin::SerialInfo message{.toolID = 0xAAu, .action = 0x55u};

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.action, result.message.action);
  ASSERT_EQ(message.toolID, result.message.toolID);
}

TEST(Message, DecoderCvManip) {
  static_assert(Codable<ulf::mx1bin::DecoderCvManip>);

  ulf::mx1bin::DecoderCvManip message{
    .cAdr = 0x8003u, .variable = 0x55u, .value = 0xAAu};

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.cAdr, result.message.cAdr);
  ASSERT_EQ(message.variable, result.message.variable);
  ASSERT_EQ(*message.value, *result.message.value);
}

TEST(Message, DecoderCvManip_no_value) {
  static_assert(Codable<ulf::mx1bin::DecoderCvManip>);

  ulf::mx1bin::DecoderCvManip message{.cAdr = 0x8003u, .variable = 0x55u};

  auto const result{encode_and_decode(message)};
  match_head(message, result.message);
  ASSERT_EQ(message.cAdr, result.message.cAdr);
  ASSERT_EQ(message.variable, result.message.variable);
  ASSERT_FALSE(message.value);
  ASSERT_FALSE(result.message.value);
}

// TEST(Message, DecoderCvManip_Reply) {}

// TEST(Message, DecoderCvManip_Busy) {}

// TEST(Message, DecoderCvManip_Erro) {}
