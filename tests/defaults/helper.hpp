#pragma once

#include <gtest/gtest.h>
#include "../helper.hpp"

template<Codable T>
void check_info(T const& t) {
  ASSERT_EQ(t.head.info.sender, ulf::mx1bin::Sender::CommandStation);
  ASSERT_EQ(t.head.info.stationType, ulf::mx1bin::StationType::MX1);

  if constexpr (ulf::mx1bin::Long<T>) {
    // Long frame
    ASSERT_EQ(t.head.info.frameType, ulf::mx1bin::FrameType::Long);
  } else if constexpr (ulf::mx1bin::Short<T>) {
    // Short frame
    ASSERT_EQ(t.head.info.frameType, ulf::mx1bin::FrameType::Short);
  } else {
    static_assert(false, "Type does not match pattern, this may be a bug");
  }

  if constexpr (IsPrimary<T>) {
    // Primary message
    ASSERT_EQ(t.head.info.messageType, ulf::mx1bin::MessageType::Primary);
  } else if constexpr (IsL1Ack<T>) {
    // L1Ack
    ASSERT_EQ(t.head.info.messageType, ulf::mx1bin::MessageType::L1Ack);
  } else if constexpr (IsReplyL2<T>) {
    // ReplyL2
    ASSERT_EQ(t.head.info.messageType, ulf::mx1bin::MessageType::ReplyL2);
  } else {
    static_assert(false, "Type does not match pattern, this may be a bug");
  }
}

template<ulf::mx1bin::Command C, Codable T>
void check_code(T const& t) {
  ASSERT_EQ(t.head.code, C);
}

template<ulf::mx1bin::Command C, Codable T>
void check() {
  T message{};

  check_info(message);
  check_code<C>(message);
}
