#include <gtest/gtest.h>
#include "helper.hpp"
#include "ulf/mx1bin.hpp"

TEST(Default, LocoMemoryQuery) {
  check<ulf::mx1bin::Command::LocoMemoryQuery, ulf::mx1bin::LocoMemoryQuery>();
}

TEST(Default, LocoMemoryQuery_Reply) {
  check<ulf::mx1bin::Command::LocoMemoryQuery,
        ulf::mx1bin::LocoMemoryQuery::Reply>();
}
