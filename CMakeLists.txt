cmake_minimum_required(VERSION 3.20)
project(ZeroMQPlayground)

set(CMAKE_CXX_STANDARD 20)
include_directories(common)
add_subdirectory(Ch1_Basics)
add_subdirectory(Ch2_SocketsandPatterns)
add_subdirectory(monitoring)
