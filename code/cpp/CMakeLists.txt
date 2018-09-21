cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_CXX_STANDARD 14)

# Naoqi needs g++ 4.8
set(CMAKE_CXX_COMPILER g++-4.8)

# Project name
project(OrangeBall)

# This include enable you to use qibuild framework
find_package(OpenCV REQUIRED)
find_package(qibuild)

# Create executable
qi_create_bin(OrangeBall orange_ball.cpp)

# Tell CMake the naoqi dependencies
qi_use_lib(OrangeBall ALCOMMON ALPROXIES)

# Link with opencv
target_link_libraries(OrangeBall ${OpenCV_LIBS})
