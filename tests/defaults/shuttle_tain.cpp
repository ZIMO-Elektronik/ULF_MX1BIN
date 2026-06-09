#include <gtest/gtest.h>
#include "helper.hpp"
#include "ulf/mx1bin.hpp"

TEST(Default, ShuttleTrain) {
  check<ulf::mx1bin::Command::ShuttleTrain, ulf::mx1bin::ShuttleTrain>();
}

TEST(Default, ShuttleTrain_Reply) {
  check<ulf::mx1bin::Command::ShuttleTrain, ulf::mx1bin::ShuttleTrain::Reply>();
}