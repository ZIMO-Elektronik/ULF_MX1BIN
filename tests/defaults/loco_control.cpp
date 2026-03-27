#include <gtest/gtest.h>
#include "helper.hpp"
#include "ulf/mx1bin.hpp"

TEST(Default, LocoControl) {
  check<ulf::mx1bin::Command::LocoControl, ulf::mx1bin::LocoControl>();
}

TEST(Default, LocoControl_Reply) {
  check<ulf::mx1bin::Command::LocoControl, ulf::mx1bin::LocoControl::Reply>();
}
