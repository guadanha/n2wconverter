cmake_minimum_required(VERSION 2.8)

project(n2wconverter)

SET(CMAKE_INSTALL_PREFIX /opt/certi/)

add_definitions(-O0 -std=c++0x -Wall -Wno-deprecated-declarations)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB COMMON_SRCS "*.cc")
add_executable(n2wconverter ${COMMON_SRCS})
target_link_libraries(n2wconverter pthread)

install(TARGETS n2wconverter DESTINATION bin)
install(CODE "execute_process(COMMAND /bin/bash
    ${CMAKE_SOURCE_DIR}/scripts/post_install.sh ${CMAKE_SOURCE_DIR})")


