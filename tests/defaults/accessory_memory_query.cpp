#include <gtest/gtest.h>
#include "helper.hpp"
#include "ulf/mx1bin.hpp"

TEST(Default, AccessoryMemoryQuery) {
  check<ulf::mx1bin::Command::AccessoryMemoryQuery,
        ulf::mx1bin::AccessoryMemoryQuery>();
}

TEST(Default, AccessoryMemoryQuery_Reply) {
  check<ulf::mx1bin::Command::AccessoryMemoryQuery,
        ulf::mx1bin::AccessoryMemoryQuery::Reply>();
}