cmake_minimum_required(VERSION 3.12)

project(CimplLang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

file(GLOB cimpl_SRC CONFIGURE DEPENDS "src/*.hpp" "src/*.cpp")

add_executable(cimpl ${cimpl_SRC})
target_link_libraries(cimpl ${CURSES_LIBRARIES})
