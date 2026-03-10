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

  ulf::mx1bin::bitfields::Info result{value};
  ASSERT_EQ(result, info);
}
