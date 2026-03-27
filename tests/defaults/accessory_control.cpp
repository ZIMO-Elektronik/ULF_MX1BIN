#include <gtest/gtest.h>
#include "helper.hpp"
#include "ulf/mx1bin.hpp"

TEST(Default, AccessoryControl) {
  check<ulf::mx1bin::Command::AccessoryControl,
        ulf::mx1bin::AccessoryControl>();
}

TEST(Default, AccessoryControl_Reply) {
  check<ulf::mx1bin::Command::AccessoryControl,
        ulf::mx1bin::AccessoryControl::Reply>();
}
