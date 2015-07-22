cmake_minimum_required(VERSION 3.2)
project(Proj2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Client.cpp
    Server.cpp
    Socket.cpp
    Socket.h)

add_executable(Proj2 ${SOURCE_FILES})