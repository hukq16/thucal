cmake_minimum_required(VERSION 3.5)
project(thucal)

set(CMAKE_CXX_STANDARD 11)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(thucal main.cpp cal.cpp cal.h TodoThing.cpp TodoThing.h)

target_link_libraries(thucal ${CURSES_LIBRARIES})