include(GoogleTest)

file(GLOB_RECURSE SRC *.cpp)
add_executable(ULF_MX1BINTests ${SRC})

sanitize(address,undefined)

target_common_warnings(ULF_MX1BINTests PRIVATE)
target_common_errors(ULF_MX1BINTests PRIVATE -Werror)

cpmaddpackage(URI "gh:google/googletest#main" OPTIONS "INSTALL_GTEST OFF")

target_link_libraries(ULF_MX1BINTests PRIVATE ULF::MX1BIN GTest::gtest_main
                                              GTest::gmock)

gtest_discover_tests(ULF_MX1BINTests)
