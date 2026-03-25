#include <gtest/gtest.h>
#include <ulf/mx1bin.hpp>

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
  ASSERT_EQ(var.info.frameType, d_##var.info.frameType);                       \
  ASSERT_EQ(var.info.messageType, d_##var.info.messageType);                   \
  ASSERT_EQ(var.info.sender, d_##var.info.sender);                             \
  ASSERT_EQ(var.info.stationType, d_##var.info.stationType);                   \
  ASSERT_EQ(var.code, d_##var.code);

template<ulf::mx1bin::Encodable E>
constexpr ulf::mx1bin::Packet encode(E& e) {
  ulf::mx1bin::Packet result;
  ulf::mx1bin::StreamEncoder en{result};
  en = e.encode(en);
  return result;
}

TEST(Message, Ack) {
  CODABLE(ulf::mx1bin::Ack<ulf::mx1bin::Command::DecoderCvManip>)

  ulf::mx1bin::Ack<ulf::mx1bin::Command::DecoderCvManip> ack{
    {.uSID = 0x00u, .info = 0x01u, .reply_uSID = 0x03u}};

  CODE(ack)

  MATCH_HEAD(ack)
  ASSERT_EQ(ack.reply_uSID, d_ack.reply_uSID);
}

TEST(Message, Nak) {
  CODABLE(ulf::mx1bin::Nak)

  ulf::mx1bin::Nak nak{{.uSID = 0x00u, .info = 0x01u}};

  CODE(nak)

  MATCH_HEAD(nak)
}

TEST(Message, Reset) {
  CODABLE(ulf::mx1bin::Reset)

  ulf::mx1bin::Reset reset{{.uSID = 0x00u, .info = 0x01u}};

  CODE(reset)

  MATCH_HEAD(reset)
}

TEST(Message, TrackControl) {
  CODABLE(ulf::mx1bin::TrackControl)

  ulf::mx1bin::TrackControl ctrl{{.uSID = 0x00u, .info = 0x01u}};
  ctrl.cAction = decltype(ctrl)::Action::TrackOn;

  CODE(ctrl)

  MATCH_HEAD(ctrl)
  ASSERT_EQ(ctrl.cAction, d_ctrl.cAction);
}

TEST(Message, TrackControl_Reply) {
  CODABLE(ulf::mx1bin::TrackControl::Reply)

  ulf::mx1bin::TrackControl::Reply reply{
    {.uSID = 0x00u, .info = 0x01u, .reply_uSID = 0x03u}};
  reply.statusBits = 0x55u;

  CODE(reply)

  MATCH_HEAD(reply)
  ASSERT_EQ(reply.statusBits, d_reply.statusBits);
}

TEST(Message, LocoControl) {
  CODABLE(ulf::mx1bin::LocoControl)

  ulf::mx1bin::LocoControl ctrl{{{.uSID = 0x00u, .info = 0x01u}}};
  ctrl.cAdr = 0x8003u;
  ctrl.cSpeed = 0xAAu;
  ctrl.cData1 = 0x55u;
  ctrl.cData2 = 0xAAu;
  ctrl.cData3 = 0x55u;

  CODE(ctrl)

  MATCH_HEAD(ctrl)
  ASSERT_EQ(ctrl.cAdr, d_ctrl.cAdr);
  ASSERT_EQ(ctrl.cSpeed, d_ctrl.cSpeed);
  ASSERT_EQ(*ctrl.cData1, *d_ctrl.cData1);
  ASSERT_EQ(*ctrl.cData2, *d_ctrl.cData2);
  ASSERT_EQ(*ctrl.cData3, *d_ctrl.cData3);
  ASSERT_FALSE(d_ctrl.cData4);
  ASSERT_FALSE(d_ctrl.cData5);
}

TEST(Message, LocoControl_Reply) {
  CODABLE(ulf::mx1bin::LocoControl::Reply)

  ulf::mx1bin::LocoControl::Reply reply{
    {{{.uSID = 0x00u, .info = 0x01u, .reply_uSID = 0x03u}}}};
  reply.error = ulf::mx1bin::Error::NO_ERROR;
  reply.payload = 0x8Cu;

  CODE(reply)

  MATCH_HEAD(reply)
  ASSERT_EQ(reply.reply_uSID, d_reply.reply_uSID);
  ASSERT_EQ(reply.error, d_reply.error);
  ASSERT_EQ(reply.payload, d_reply.payload);
}

TEST(Message, InvertFunctionBits) {
  CODABLE(ulf::mx1bin::InvertFunctionBits)

  ulf::mx1bin::InvertFunctionBits msg{{{.uSID = 0x00u, .info = 0x01u}}};

  msg.cAdr = 0x8003u;
  msg.cData1 = 0x55u;
  msg.cData2 = 0xAAu;
  msg.cData3 = 0x55u;
  msg.cData4 = 0xAAu;
  msg.cData5 = 0x55u;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.cAdr, d_msg.cAdr);
  ASSERT_EQ(msg.cData1, d_msg.cData1);
  ASSERT_EQ(msg.cData2, d_msg.cData2);
  ASSERT_EQ(msg.cData3, d_msg.cData3);
  ASSERT_EQ(msg.cData4, d_msg.cData4);
  ASSERT_EQ(msg.cData5, d_msg.cData5);
}

TEST(Message, InvertFunctionBits_Reply) {
  CODABLE(ulf::mx1bin::InvertFunctionBits::Reply)

  ulf::mx1bin::InvertFunctionBits::Reply reply{
    {{{.uSID = 0x00u, .info = 0x01u, .reply_uSID = 0x03u}}}};
  reply.error = ulf::mx1bin::Error::NO_ERROR;
  reply.payload = 0x8Cu;

  CODE(reply)

  MATCH_HEAD(reply)
  ASSERT_EQ(reply.reply_uSID, d_reply.reply_uSID);
  ASSERT_EQ(reply.error, d_reply.error);
  ASSERT_EQ(reply.payload, d_reply.payload);
}

TEST(Message, Acceleration) {
  CODABLE(ulf::mx1bin::Acceleration)

  ulf::mx1bin::Acceleration msg{{{.uSID = 0x00u, .info = 0x01u}}};

  msg.cAdr = 0x8003u;
  msg.cAzBz = 0x55u;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.cAdr, d_msg.cAdr);
  ASSERT_EQ(msg.cAzBz, d_msg.cAzBz);
}

TEST(Message, Acceleration_Reply) {
  CODABLE(ulf::mx1bin::Acceleration::Reply)

  ulf::mx1bin::Acceleration::Reply reply{
    {{{.uSID = 0x00u, .info = 0x01u, .reply_uSID = 0x03u}}}};
  reply.error = ulf::mx1bin::Error::NO_ERROR;
  reply.payload = 0x8Cu;

  CODE(reply)

  MATCH_HEAD(reply)
  ASSERT_EQ(reply.reply_uSID, d_reply.reply_uSID);
  ASSERT_EQ(reply.error, d_reply.error);
  ASSERT_EQ(reply.payload, d_reply.payload);
}

TEST(Message, ShuttleTrain) {
  CODABLE(ulf::mx1bin::ShuttleTrain)

  ulf::mx1bin::ShuttleTrain msg{{{{.uSID = 0x00u, .info = 0x01u}}}};

  msg.cAdr = 0x8003u;
  msg.cData = 0x55u;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.cAdr, d_msg.cAdr);
  ASSERT_EQ(msg.cData, d_msg.cData);
}

TEST(Message, ShuttleTrain_Reply) {
  CODABLE(ulf::mx1bin::ShuttleTrain::Reply)

  ulf::mx1bin::ShuttleTrain::Reply reply{
    {{{.uSID = 0x00u, .info = 0x01u, .reply_uSID = 0x03u}}}};
  reply.error = ulf::mx1bin::Error::NO_ERROR;
  reply.payload = 0x8Cu;

  CODE(reply)

  MATCH_HEAD(reply)
  ASSERT_EQ(reply.reply_uSID, d_reply.reply_uSID);
  ASSERT_EQ(reply.error, d_reply.error);
  ASSERT_EQ(reply.payload, d_reply.payload);
}

TEST(Message, AccessoryControl) {
  CODABLE(ulf::mx1bin::AccessoryControl)

  ulf::mx1bin::AccessoryControl msg{{{{.uSID = 0x00u, .info = 0x01u}}}};

  msg.cAdr = 0x8003u;
  msg.cData = 0x55u;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.cAdr, d_msg.cAdr);
  ASSERT_EQ(msg.cData, d_msg.cData);
}

TEST(Message, AccessoryControl_Reply) {
  CODABLE(ulf::mx1bin::AccessoryControl::Reply)

  ulf::mx1bin::AccessoryControl::Reply reply{
    {{{.uSID = 0x00u, .info = 0x01u, .reply_uSID = 0x03u}}}};
  reply.error = ulf::mx1bin::Error::NO_ERROR;
  reply.payload = 0x8Cu;

  CODE(reply)

  MATCH_HEAD(reply)
  ASSERT_EQ(reply.reply_uSID, d_reply.reply_uSID);
  ASSERT_EQ(reply.error, d_reply.error);
  ASSERT_EQ(reply.payload, d_reply.payload);
}

TEST(Message, LocoMemoryQuery) {
  CODABLE(ulf::mx1bin::LocoMemoryQuery)

  ulf::mx1bin::LocoMemoryQuery msg{{{.uSID = 0x00u, .info = 0x01u}}};

  msg.cAdr = 0x8003u;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.cAdr, d_msg.cAdr);
}

TEST(Message, LocoMemoryQuery_Reply) {
  CODABLE(ulf::mx1bin::LocoMemoryQuery::Reply)

  ulf::mx1bin::LocoMemoryQuery::Reply reply{
    {{.uSID = 0x00u, .info = 0x01u, .reply_uSID = 0x03u}}};
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
  ASSERT_EQ(reply.reply_uSID, d_reply.reply_uSID);
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

  ulf::mx1bin::AccessoryMemoryQuery msg{{{.uSID = 0x00u, .info = 0x01u}}};

  msg.cAdr = 0x8003u;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.cAdr, d_msg.cAdr);
}

TEST(Message, AccessoryMemoryQuery_Reply) {
  CODABLE(ulf::mx1bin::AccessoryMemoryQuery::Reply)

  ulf::mx1bin::AccessoryMemoryQuery::Reply msg{
    {{.uSID = 0x00u, .info = 0x01u, .reply_uSID = 0x04u}}};

  msg.error = ulf::mx1bin::Error::NO_ERROR;
  msg.cAdr = 0x8003u;
  msg.cPair = 0x55u;
  msg.cOutputs = 0xAAu;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.error, d_msg.error);
  ASSERT_EQ(msg.cAdr, d_msg.cAdr);
  ASSERT_EQ(msg.cPair, d_msg.cPair);
  ASSERT_EQ(msg.cOutputs, d_msg.cOutputs);
}

TEST(Message, AddressControl) {
  CODABLE(ulf::mx1bin::AddressControl)

  ulf::mx1bin::AddressControl msg{{{.uSID = 0x00u, .info = 0x01u}}};

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

  ulf::mx1bin::AddressControl::Reply msg{
    {.uSID = 0x00u, .info = 0x01u, .reply_uSID = 0x04u}};

  msg.payload = 0x55u;
  msg.cOutputs = 0xAAu;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.payload, d_msg.payload);
  ASSERT_EQ(msg.cOutputs, d_msg.cOutputs);
}

TEST(Message, CommandStationIOQuery) {
  CODABLE(ulf::mx1bin::CommandStationIOQuery)

  ulf::mx1bin::CommandStationIOQuery msg{{{.uSID = 0x00u, .info = 0x01u}}};

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.zero, d_msg.zero);
}

TEST(Message, CommandStationIOQuery_Reply) {
  CODABLE(ulf::mx1bin::CommandStationIOQuery::Reply)

  ulf::mx1bin::CommandStationIOQuery::Reply msg{
    {.uSID = 0x00u, .info = 0x01u, .reply_uSID = 0x04u}};

  msg.cCurrent1 = 0x55u;
  msg.cVoltage1 = 0xAAu;
  msg.cCurrent2 = 0x55u;
  msg.cVoltage2 = 0xAAu;
  msg.cAux = 0x55u;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.cCurrent1, d_msg.cCurrent1);
  ASSERT_EQ(msg.cVoltage1, d_msg.cVoltage1);
  ASSERT_EQ(msg.cCurrent2, d_msg.cCurrent2);
  ASSERT_EQ(msg.cVoltage2, d_msg.cVoltage2);
  ASSERT_EQ(msg.cAux, d_msg.cAux);
}

TEST(Message, CommandStationCvManip) {
  CODABLE(ulf::mx1bin::CommandStationCvManip)

  ulf::mx1bin::CommandStationCvManip msg{{.uSID = 0x00u, .info = 0x01u}};

  msg.variable = 0x55u;
  msg.value = 0xAAu;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.variable, d_msg.variable);
  ASSERT_EQ(*msg.value, *d_msg.value);
}

TEST(Message, CommandStationCvManip_Reply) {
  CODABLE(ulf::mx1bin::CommandStationCvManip::Reply)

  ulf::mx1bin::CommandStationCvManip::Reply msg{
    {{.uSID = 0x00u, .info = 0x01u, .reply_uSID = 0x04u}}};

  msg.error = ulf::mx1bin::Error::NO_ERROR;
  msg.value = 0x55u;

  CODE(msg)

  MATCH_HEAD(msg)
  ASSERT_EQ(msg.error, d_msg.error);
  ASSERT_EQ(msg.value, d_msg.value);
}

TEST(Message, CommandStationEquipmentQuery) {
  CODABLE(ulf::mx1bin::CommandStationEquipmentQuery)

  ulf::mx1bin::CommandStationEquipmentQuery msg{
    {{.uSID = 0x00u, .info = 0x01u}}};

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

  ulf::mx1bin::DecoderCvManip msg{{{.uSID = 0x00u, .info = 0x01u}}};

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
