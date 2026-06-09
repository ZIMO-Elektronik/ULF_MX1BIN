#include <gtest/gtest.h>
#include "helper.hpp"
#include "ulf/mx1bin.hpp"

TEST(Default, Acceleration) {
  check<ulf::mx1bin::Command::Acceleration, ulf::mx1bin::Acceleration>();
}

TEST(Default, Acceleration_Reply) {
  check<ulf::mx1bin::Command::Acceleration, ulf::mx1bin::Acceleration::Reply>();
}
