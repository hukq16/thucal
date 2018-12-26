cmake_minimum_required(VERSION 3.5)
project(thucal)

set(CMAKE_CXX_STANDARD 11)
set(LIBXL_LIB /usr/lib/libxl.so)
set(CDK_LIBRARIES /usr/local/lib/libcdk.a)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
link_libraries(${CDK_LIBRARIES})
message(STATUS "${CURSES_INCLUDE_DIR}")
message(STATUS "${CURSES_LIBRARIES}")
add_executable(thucal main.cpp Todolist.cpp Todolist.h Caltime.cpp Caltime.h Vevent.cpp Vevent.h VeventRepeat.cpp VeventRepeat.h VeventNoRepeat.cpp VeventNoRepeat.h Cal.cpp Cal.h)

target_link_libraries(thucal ${CURSES_LIBRARIES} ${LIBXL_LIB})