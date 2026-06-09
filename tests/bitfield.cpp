#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ulf/mx1bin.hpp>
#include <ulf/mx1bin/bitfields.hpp>
#include <ulf/mx1bin/info.hpp>

TEST(bitfield, header_info) {
  ulf::mx1bin::bitfields::Info info{};
  info.frameType = ulf::mx1bin::FrameType::Long;
  info.messageType = ulf::mx1bin::MessageType::Primary;
  info.sender = ulf::mx1bin::Sender::PC;
  info.stationType = ulf::mx1bin::StationType::MX1;

  auto value{static_cast<uint8_t>(info)};
  ASSERT_EQ(value, 0b10010000u);

  decltype(info) result{value};
  ASSERT_EQ(result, info);
}

TEST(bitfield, DecoderAddress) {
  ulf::mx1bin::bitfields::DecoderAddress address{};
  address.format = 0b10u;
  address.address = 3u;

  auto const value{static_cast<uint16_t>(address)};
  ASSERT_EQ(value, 0b1000'0000'0000'0011u);

  decltype(address) result{value};
  ASSERT_EQ(result, address);
}

TEST(bitfield, TrackStatus) {
  ulf::mx1bin::bitfields::TrackStatus status{};
  status.dcc = 1u;
  status.motorola = 0u;
  status.ues = 0u;
  status.trackVoltage = 1u;
  status.broadcastStop = 0u;

  auto const value{static_cast<uint8_t>(status)};
  ASSERT_EQ(value, 0b1000'0010u);

  decltype(status) result{value};
  ASSERT_EQ(result, status);
}

TEST(bitfield, ControlSpeed) {
  ulf::mx1bin::bitfields::ControlSpeed ctrl{};
  ctrl.stop = 0b1u;
  ctrl.speed = 0b110011u;

  auto const value{static_cast<uint8_t>(ctrl)};
  ASSERT_EQ(value, 0b1011'0011u);

  decltype(ctrl) result{value};
  ASSERT_EQ(result, ctrl);
}

TEST(bitfield, ControlPayload) {
  ulf::mx1bin::bitfields::ControlPayload ctrl{};
  ctrl.format = 0b10u;
  ctrl.speedStep = ulf::mx1bin::SpeedStep::_126; // = 0b11u
  ctrl.trackState = 0b1u;

  auto const value{static_cast<uint8_t>(ctrl)};
  ASSERT_EQ(value, 0b1000'1101u);

  decltype(ctrl) result{value};
  ASSERT_EQ(result, ctrl);
}

TEST(bitfields, ControlData) {
  ulf::mx1bin::bitfields::ControlData ctrl{};
  ctrl.manual = 0b1u;
  ctrl.direction = 0b0u;
  ctrl.headlights = 0b1u;
  ctrl.speed_step = ulf::mx1bin::SpeedStep::_126; // = 0b11u
  ctrl.az_enable = 0b0u;
  ctrl.bz_enable = 0b1u;

  auto const value{static_cast<uint8_t>(ctrl)};
  ASSERT_EQ(value, 0b1001'1101u);

  decltype(ctrl) result{value};
  ASSERT_EQ(result, ctrl);
}

TEST(bitfields, AddressControl_Control) {
  ulf::mx1bin::bitfields::AddressControl_Control adr_ctrl{};
  adr_ctrl.set = 1u;
  adr_ctrl.type = 1u;
  adr_ctrl.lock = 1u;
  adr_ctrl.log = 1u;

  auto const value{static_cast<uint8_t>(adr_ctrl)};
  ASSERT_EQ(value, 0b1010'0011u);

  decltype(adr_ctrl) result{value};
  ASSERT_EQ(result, adr_ctrl);
}

TEST(bitfields, AddressControl_Payload) {
  ulf::mx1bin::bitfields::AddressControl_Payload adr_payload{};
  adr_payload.format = 0b10u;
  adr_payload.type = 0b1u;
  adr_payload.speedStep = ulf::mx1bin::SpeedStep::_126; // = 0b11u
  adr_payload.lock = 0b1u;
  adr_payload.log = 0b1u;

  auto const value{static_cast<uint8_t>(adr_payload)};
  ASSERT_EQ(value, 0b1010'1111u);

  decltype(adr_payload) result{value};
  ASSERT_EQ(result, adr_payload);
}
