#include <gtest/gtest.h>
#include "helper.hpp"
#include "ulf/mx1bin.hpp"

TEST(Default, Nak) { check<ulf::mx1bin::Command::Nak, ulf::mx1bin::Nak>(); }
