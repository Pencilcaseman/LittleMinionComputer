cmake_minimum_required(VERSION 3.27)
project(LittleMinionComputer)

set(CMAKE_CXX_STANDARD 23)

add_executable(LittleMinionComputer main.cpp)

add_subdirectory(surge)
target_link_libraries(LittleMinionComputer surge)

target_include_directories(LittleMinionComputer PUBLIC include)
