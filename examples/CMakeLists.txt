file(GLOB_RECURSE SRC *.cpp)
add_executable(MX1BinExamples ${SRC})

target_common_warnings(MX1BinExamples PRIVATE)

target_link_libraries(MX1BinExamples PRIVATE ULF::MX1Bin)
