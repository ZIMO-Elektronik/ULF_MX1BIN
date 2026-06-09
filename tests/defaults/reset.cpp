#include <gtest/gtest.h>
#include "helper.hpp"
#include "ulf/mx1bin.hpp"

TEST(Default, Reset) {
  check<ulf::mx1bin::Command::Reset, ulf::mx1bin::Reset>();
}

TEST(Default, Reset_Reply) {
  check<ulf::mx1bin::Command::Reset, ulf::mx1bin::Reset::Reply>();
}
