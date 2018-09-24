cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_CXX_STANDARD 14)

# Naoqi needs g++ 4.8
set(CMAKE_CXX_COMPILER g++-4.8)

# Project name
project(Balldetector)

# This include enable you to use qibuild framework
find_package(OpenCV REQUIRED)
find_package(qibuild)

# Create executable
qi_create_bin(BallDetector main.cpp orange_ball.cpp ball_tracker.cpp)

# Tell CMake the naoqi dependencies
qi_use_lib(BallDetector ALCOMMON ALPROXIES)

# Link with opencv
target_link_libraries(BallDetector ${OpenCV_LIBS})
