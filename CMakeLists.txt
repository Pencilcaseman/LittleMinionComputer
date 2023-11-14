cmake_minimum_required(VERSION 3.27)
project(LittleMinionComputer)

set(CMAKE_CXX_STANDARD 23)

add_executable(LittleMinionComputer main.cpp
        include/lmc/assembler.hpp)

add_subdirectory(librapid)
add_subdirectory(scnlib)
target_link_libraries(LittleMinionComputer PUBLIC librapid scn)

target_include_directories(LittleMinionComputer PUBLIC include)
