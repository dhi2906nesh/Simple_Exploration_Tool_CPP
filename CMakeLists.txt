cmake_minimum_required(VERSION 3.10)
project(LoopVisualizer)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

add_executable(loop_visualizer
    src/main.cpp
    src/Parser.cpp
    src/Visualizer.cpp
    src/Utils.cpp
)
