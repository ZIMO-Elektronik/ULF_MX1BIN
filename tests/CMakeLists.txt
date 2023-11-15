include(GoogleTest)

file(GLOB_RECURSE SRC *.cpp)
add_executable(MX1BinTests ${SRC})

sanitize("address,undefined")

target_common_warnings(MX1BinTests PRIVATE)

cpmaddpackage("gh:hanickadot/compile-time-regular-expressions@3.8.1")
cpmaddpackage("gh:google/googletest#main")

target_link_libraries(MX1BinTests PRIVATE ctre::ctre GTest::gtest_main
                                          GTest::gmock)

gtest_discover_tests(MX1BinTests)
