
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Link runTests with what we want to test and the GTest and pthread library
add_executable(unit-test "unit-test.cc" "parser_test.h" "../parser.cc" "client_reply_test.h" "config_file_test.h" "../config_file.cc")
target_link_libraries(unit-test ${GTEST_LIBRARIES} pthread)
