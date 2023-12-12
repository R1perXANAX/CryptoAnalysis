cmake_minimum_required(VERSION 3.27)
project(ADFGVXCrackerV2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ADFGVXCrackerV2
        ADFGVXCrackerV2.cpp
        Cipher.cpp
        Cipher.h
        Cracker.cpp
        Cracker.h)
