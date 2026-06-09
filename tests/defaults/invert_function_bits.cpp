#include <gtest/gtest.h>
#include "helper.hpp"
#include "ulf/mx1bin.hpp"

TEST(Default, InvertFunctionBits) {
  check<ulf::mx1bin::Command::InvertFunctionBits,
        ulf::mx1bin::InvertFunctionBits>();
}

TEST(Default, InvertFunctionBits_Reply) {
  check<ulf::mx1bin::Command::InvertFunctionBits,
        ulf::mx1bin::InvertFunctionBits::Reply>();
}