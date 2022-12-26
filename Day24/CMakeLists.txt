cmake_minimum_required(VERSION 3.17)
project(AdventofCode)

set(CMAKE_CXX_STANDARD 20)
include(${CMAKE_BINARY_DIR}/../build/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)
add_executable(day24 day24.cpp ../common.h)
target_link_libraries(day24 CONAN_PKG::range-v3)



