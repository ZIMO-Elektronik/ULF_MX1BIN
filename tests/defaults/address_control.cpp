#include <gtest/gtest.h>
#include "helper.hpp"
#include "ulf/mx1bin.hpp"

TEST(Default, AddressControl) {
  check<ulf::mx1bin::Command::AddressControl, ulf::mx1bin::AddressControl>();
}

TEST(Default, AddressControl_Reply) {
  check<ulf::mx1bin::Command::AddressControl,
        ulf::mx1bin::AddressControl::Reply>();
}
