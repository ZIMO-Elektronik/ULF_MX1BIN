file(GLOB_RECURSE SRC *.cpp)
add_executable(ULF_MX1BINExamples ${SRC})

target_common_warnings(ULF_MX1BINExamples PRIVATE)
target_common_errors(ULF_MX1BINExamples PRIVATE)

target_link_libraries(ULF_MX1BINExamples PRIVATE ULF::MX1BIN)
