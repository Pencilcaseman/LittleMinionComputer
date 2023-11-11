cmake_minimum_required(VERSION 3.27)
project(LittleMinionComputer)

set(CMAKE_CXX_STANDARD 23)

add_executable(LittleMinionComputer
        main.cpp
        src/datum.cpp
        src/programState.cpp
        include/lmc/littleMinionComputer.hpp
        src/littleMinionComputer.cpp
        include/lmc/instructionSet.hpp
        src/instructionSet.cpp
)

add_subdirectory(surge)
target_link_libraries(LittleMinionComputer surge)

target_include_directories(LittleMinionComputer PUBLIC include)
