#include <gtest/gtest.h>
#include <ulf/mx1bin.hpp>
#include "helper.hpp"

TEST(Message, Ack) {
  static_assert(Codable<ulf::mx1bin::Ack>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::Ack>);

  ulf::mx1bin::Ack message{};

  message.head.reply_uSID = 0x03u;

  encode_decode_encode(message);
}

TEST(Message, Nak) {
  static_assert(Codable<ulf::mx1bin::Nak>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::Nak>);

  ulf::mx1bin::Nak message{};

  encode_decode_encode(message);
}

TEST(Message, Reset) {
  static_assert(Codable<ulf::mx1bin::Reset>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::Reset>);

  ulf::mx1bin::Reset message{};

  encode_decode_encode(message);
}

TEST(Message, TrackControl) {
  static_assert(Codable<ulf::mx1bin::TrackControl>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::TrackControl>);

  ulf::mx1bin::TrackControl message{.cAction =
                                      decltype(message)::Action::TrackOn};

  encode_decode_encode(message);
}

TEST(Message, TrackControl_Reply) {
  static_assert(Codable<ulf::mx1bin::TrackControl::Reply>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::TrackControl::Reply>);

  ulf::mx1bin::TrackControl::Reply message{.statusBits = 0x55u};
  message.head.reply_uSID = 0x03u;

  encode_decode_encode(message);
}

TEST(Message, LocoControl) {
  static_assert(Codable<ulf::mx1bin::LocoControl>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::LocoControl>);

  ulf::mx1bin::LocoControl message{.cAdr = 0x8003u,
                                   .cSpeed = 0xAAu,
                                   .cData1 = 0x55u,
                                   .cData2 = 0xAAu,
                                   .cData3 = 0x55u};

  encode_decode_encode(message);
}

TEST(Message, LocoControl_Reply) {
  static_assert(Codable<ulf::mx1bin::LocoControl::Reply>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::LocoControl::Reply>);

  ulf::mx1bin::LocoControl::Reply message{.error = ulf::mx1bin::Error::NO_ERROR,
                                          .payload = 0x8Cu};
  message.head.reply_uSID = 0x03u;

  encode_decode_encode(message);
}

TEST(Message, InvertFunctionBits) {
  static_assert(Codable<ulf::mx1bin::InvertFunctionBits>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::InvertFunctionBits>);

  ulf::mx1bin::InvertFunctionBits message{.cAdr = 0x8003u,
                                          .cData1 = 0x55u,
                                          .cData2 = 0xAAu,
                                          .cData3 = 0x55u,
                                          .cData4 = 0xAAu,
                                          .cData5 = 0x55u};

  encode_decode_encode(message);
}

TEST(Message, InvertFunctionBits_Reply) {
  static_assert(Codable<ulf::mx1bin::InvertFunctionBits::Reply>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::InvertFunctionBits::Reply>);

  ulf::mx1bin::InvertFunctionBits::Reply message{
    .error = ulf::mx1bin::Error::NO_ERROR, .payload = 0x8Cu};
  message.head.reply_uSID = 0x03u;

  encode_decode_encode(message);
}

TEST(Message, Acceleration) {
  static_assert(Codable<ulf::mx1bin::Acceleration>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::Acceleration>);

  ulf::mx1bin::Acceleration message{.cAdr = 0x8003u, .cAzBz = 0x55u};

  encode_decode_encode(message);
}

TEST(Message, Acceleration_Reply) {
  static_assert(Codable<ulf::mx1bin::Acceleration::Reply>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::Acceleration::Reply>);

  ulf::mx1bin::Acceleration::Reply message{
    .error = ulf::mx1bin::Error::NO_ERROR, .payload = 0x8Cu};
  message.head.reply_uSID = 0x03u;

  encode_decode_encode(message);
}

TEST(Message, ShuttleTrain) {
  static_assert(Codable<ulf::mx1bin::ShuttleTrain>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::ShuttleTrain>);

  ulf::mx1bin::ShuttleTrain message{.cAdr = 0x8003u, .cData = 0x55u};

  encode_decode_encode(message);
}

TEST(Message, ShuttleTrain_Reply) {
  static_assert(Codable<ulf::mx1bin::ShuttleTrain::Reply>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::ShuttleTrain::Reply>);

  ulf::mx1bin::ShuttleTrain::Reply message{
    .error = ulf::mx1bin::Error::NO_ERROR, .payload = 0x8Cu};
  message.head.reply_uSID = 0x03u;

  encode_decode_encode(message);
}

TEST(Message, AccessoryControl) {
  static_assert(Codable<ulf::mx1bin::AccessoryControl>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::AccessoryControl>);

  ulf::mx1bin::AccessoryControl message{.cAdr = 0x8003u, .cData = 0x55u};

  encode_decode_encode(message);
}

TEST(Message, AccessoryControl_Reply) {
  static_assert(Codable<ulf::mx1bin::AccessoryControl::Reply>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::AccessoryControl::Reply>);

  ulf::mx1bin::AccessoryControl::Reply message{
    .error = ulf::mx1bin::Error::NO_ERROR, .payload = 0x8Cu};
  message.head.reply_uSID = 0x03u;

  encode_decode_encode(message);
}

TEST(Message, LocoMemoryQuery) {
  static_assert(Codable<ulf::mx1bin::LocoMemoryQuery>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::LocoMemoryQuery>);

  ulf::mx1bin::LocoMemoryQuery message{.cAdr = 0x8003u};

  encode_decode_encode(message);
}

TEST(Message, LocoMemoryQuery_Reply) {
  static_assert(Codable<ulf::mx1bin::LocoMemoryQuery::Reply>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::LocoMemoryQuery::Reply>);

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

  encode_decode_encode(message);
}

TEST(Message, AccessoryMemoryQuery) {
  static_assert(Codable<ulf::mx1bin::AccessoryMemoryQuery>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::AccessoryMemoryQuery>);

  ulf::mx1bin::AccessoryMemoryQuery message{.cAdr = 0x8003u};

  encode_decode_encode(message);
}

TEST(Message, AccessoryMemoryQuery_Reply) {
  static_assert(Codable<ulf::mx1bin::AccessoryMemoryQuery::Reply>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::AccessoryMemoryQuery::Reply>);

  ulf::mx1bin::AccessoryMemoryQuery::Reply message{
    .error = ulf::mx1bin::Error::NO_ERROR,
    .cAdr = 0x8003u,
    .cPair = 0x55u,
    .cOutputs = 0xAAu};
  message.head.reply_uSID = 0x03u;

  encode_decode_encode(message);
}

TEST(Message, AddressControl) {
  static_assert(Codable<ulf::mx1bin::AddressControl>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::AddressControl>);

  ulf::mx1bin::AddressControl message{
    .cAdr = 0x8003u, .cControl = 0x55u, .cOutputs = 0xAAu};

  encode_decode_encode(message);
}

TEST(Message, AddressControl_Reply) {
  static_assert(Codable<ulf::mx1bin::AddressControl::Reply>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::AddressControl::Reply>);

  ulf::mx1bin::AddressControl::Reply message{.payload = 0x55u,
                                             .cOutputs = 0xAAu};
  message.head.reply_uSID = 0x03u;

  encode_decode_encode(message);
}

TEST(Message, CommandStationIOQuery) {
  static_assert(Codable<ulf::mx1bin::CommandStationIOQuery>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::CommandStationIOQuery>);

  ulf::mx1bin::CommandStationIOQuery message{};

  encode_decode_encode(message);
}

TEST(Message, CommandStationIOQuery_Reply) {
  static_assert(Codable<ulf::mx1bin::CommandStationIOQuery::Reply>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::CommandStationIOQuery::Reply>);

  ulf::mx1bin::CommandStationIOQuery::Reply message{.cCurrent1 = 0x55u,
                                                    .cVoltage1 = 0xAAu,
                                                    .cCurrent2 = 0x55u,
                                                    .cVoltage2 = 0xAAu,
                                                    .cAux = 0x55u};
  message.head.reply_uSID = 0x03u;

  encode_decode_encode(message);
}

TEST(Message, CommandStationCvManip) {
  static_assert(Codable<ulf::mx1bin::CommandStationCvManip>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::CommandStationCvManip>);

  ulf::mx1bin::CommandStationCvManip message{.variable = 0x55u, .value = 0xAAu};

  encode_decode_encode(message);
}

TEST(Message, CommandStationCvManip_Reply) {
  static_assert(Codable<ulf::mx1bin::CommandStationCvManip::Reply>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::CommandStationCvManip::Reply>);

  ulf::mx1bin::CommandStationCvManip::Reply message{

    .error = ulf::mx1bin::Error::NO_ERROR, .value = 0x55u};
  message.head.reply_uSID = 0x03u;

  encode_decode_encode(message);
}

TEST(Message, CommandStationEquipmentQuery) {
  static_assert(Codable<ulf::mx1bin::CommandStationEquipmentQuery>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::CommandStationEquipmentQuery>);

  ulf::mx1bin::CommandStationEquipmentQuery message{};

  encode_decode_encode(message);
}

TEST(Message, CommandStationEquipmentQuery_Reply) {
  // Decoder and encoder are still missing, but we can test for message type

  /// static_assert(Codable<ulf::mx1bin::CommandStationEquipmentQuery::Reply>);
  static_assert(
    ulf::mx1bin::Long<ulf::mx1bin::CommandStationEquipmentQuery::Reply>);
}

TEST(Message, SerialInfo) {
  static_assert(Codable<ulf::mx1bin::SerialInfo>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::SerialInfo>);

  ulf::mx1bin::SerialInfo message{.toolID = 0xAAu, .action = 0x55u};

  encode_decode_encode(message);
}

TEST(Message, DecoderCvManip) {
  static_assert(Codable<ulf::mx1bin::DecoderCvManip>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::DecoderCvManip>);

  ulf::mx1bin::DecoderCvManip message{
    .cAdr = 0x8003u, .variable = 0x55u, .value = 0xAAu};

  encode_decode_encode(message);
}

TEST(Message, DecoderCvManip_no_value) {
  static_assert(Codable<ulf::mx1bin::DecoderCvManip>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::DecoderCvManip>);

  ulf::mx1bin::DecoderCvManip message{.cAdr = 0x8003u, .variable = 0x55u};

  encode_decode_encode(message);
}

TEST(Message, DecoderCvManip_Reply) {
  static_assert(Codable<ulf::mx1bin::DecoderCvManip::Reply>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::DecoderCvManip::Reply>);

  ulf::mx1bin::DecoderCvManip::Reply message{};

  encode_decode_encode(message);
}

TEST(Message, DecoderCvManip_ReplyL2) {
  static_assert(Codable<ulf::mx1bin::DecoderCvManip::ReplyL2>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::DecoderCvManip::ReplyL2>);

  ulf::mx1bin::DecoderCvManip::ReplyL2 message{
    .cAdr = 0x8003u, .variable = 8u, .cValue = 8u};

  encode_decode_encode(message);
}

TEST(Message, DecoderCvManip_Busy) {
  static_assert(Codable<ulf::mx1bin::DecoderCvManip::Busy>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::DecoderCvManip::Busy>);

  ulf::mx1bin::DecoderCvManip::Busy message{.cAdr = 0x8003u,
                                            .variable = 8u,
                                            .activeUSID = 2u,
                                            .activeAddr = 0x8004u,
                                            .activeCv = 8u};

  encode_decode_encode(message);
}

TEST(Message, DecoderCvManip_Error) {
  static_assert(Codable<ulf::mx1bin::DecoderCvManip::Error>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::DecoderCvManip::Error>);

  ulf::mx1bin::DecoderCvManip::Error message{.cAdr = 0x8003u, .cError = 1u};

  encode_decode_encode(message);
}

TEST(Message, DecoderMultiCvManip) {
  static_assert(Codable<ulf::mx1bin::DecoderMultiCvManip>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::DecoderMultiCvManip>);

  ulf::mx1bin::DecoderMultiCvManip message{
    .cAdr = 0x8003u, .index = 3u, .variable = 7u, .sequenceID = 2u};

  encode_decode_encode(message);
}

TEST(Message, DecoderMultiCvManip_with_one_value) {
  static_assert(Codable<ulf::mx1bin::DecoderMultiCvManip>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::DecoderMultiCvManip>);

  ulf::mx1bin::DecoderMultiCvManip message{.cAdr = 0x8003u,
                                           .index = 3u,
                                           .variable = 7u,
                                           .sequenceID = 2u,
                                           .values = {1u}};

  encode_decode_encode(message);
}

TEST(Message, DecoderMultiCvManip_with_two_values) {
  static_assert(Codable<ulf::mx1bin::DecoderMultiCvManip>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::DecoderMultiCvManip>);

  ulf::mx1bin::DecoderMultiCvManip message{.cAdr = 0x8003u,
                                           .index = 3u,
                                           .variable = 7u,
                                           .sequenceID = 2u,
                                           .values = {1u, 2u}};

  encode_decode_encode(message);
}

TEST(Message, DecoderMultiCvManip_with_three_values) {
  static_assert(Codable<ulf::mx1bin::DecoderMultiCvManip>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::DecoderMultiCvManip>);

  ulf::mx1bin::DecoderMultiCvManip message{.cAdr = 0x8003u,
                                           .index = 3u,
                                           .variable = 7u,
                                           .sequenceID = 2u,
                                           .values = {1u, 2u, 3u}};

  encode_decode_encode(message);
}

TEST(Message, DecoderMultiCvManip_with_four_values) {
  static_assert(Codable<ulf::mx1bin::DecoderMultiCvManip>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::DecoderMultiCvManip>);

  ulf::mx1bin::DecoderMultiCvManip message{.cAdr = 0x8003u,
                                           .index = 3u,
                                           .variable = 7u,
                                           .sequenceID = 2u,
                                           .values = {1u, 2u, 3u, 4u}};

  encode_decode_encode(message);
}

TEST(Message, DecoderMultiCvManip_Reply) {
  static_assert(Codable<ulf::mx1bin::DecoderMultiCvManip::Reply>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::DecoderMultiCvManip::Reply>);

  ulf::mx1bin::DecoderMultiCvManip::Reply message{};

  encode_decode_encode(message);
}

TEST(Message, DecoderMultiCvManip_Busy) {
  static_assert(Codable<ulf::mx1bin::DecoderMultiCvManip::Busy>);
  static_assert(ulf::mx1bin::Long<ulf::mx1bin::DecoderMultiCvManip::Busy>);

  ulf::mx1bin::DecoderMultiCvManip::Busy message{.cAdr = 0x8003u,
                                                 .index = 3u,
                                                 .variable = 8u,
                                                 .sequenceID = 2u,
                                                 .activeUSID = 2u,
                                                 .cError = 42u,
                                                 .activeAddr = 0x8004u,
                                                 .activeIndex = 3u,
                                                 .activeCv = 8u,
                                                 .activeSequenceID = 3u};

  encode_decode_encode(message);
}

TEST(Message, DecoderMultiCvManip_ReplyL2) {
  static_assert(Codable<ulf::mx1bin::DecoderMultiCvManip::ReplyL2>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::DecoderMultiCvManip::ReplyL2>);

  ulf::mx1bin::DecoderMultiCvManip::ReplyL2 message{.cAdr = 0x8003u,
                                                    .index = 3u,
                                                    .variable = 7u,
                                                    .sequenceID = 2u,
                                                    .values = {1u, 2u, 3u, 4u},
                                                    .cError = 42u};

  encode_decode_encode(message);
}

TEST(Message, DecoderMultiCvManip_Error) {
  static_assert(Codable<ulf::mx1bin::DecoderMultiCvManip::Error>);
  static_assert(ulf::mx1bin::Short<ulf::mx1bin::DecoderMultiCvManip::Error>);

  ulf::mx1bin::DecoderMultiCvManip::Error message{.cAdr = 0x8003u,
                                                  .cError = 1u};

  encode_decode_encode(message);
}
