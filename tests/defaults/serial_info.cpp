#include <gtest/gtest.h>
#include "helper.hpp"
#include "ulf/mx1bin.hpp"

TEST(Default, SerialInfo) {
  check<ulf::mx1bin::Command::SerialInfo, ulf::mx1bin::SerialInfo>();
}

TEST(Default, SerialInfo_Reply) {
  check<ulf::mx1bin::Command::SerialInfo, ulf::mx1bin::SerialInfo::Reply>();
}