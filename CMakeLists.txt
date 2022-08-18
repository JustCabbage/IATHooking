cmake_minimum_required(VERSION 3.0.0)
project(IATHooking VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)

add_library(IATHooking SHARED IATHooking.cpp Hooking/Hooking.cpp)