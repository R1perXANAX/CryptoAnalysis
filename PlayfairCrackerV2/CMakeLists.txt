cmake_minimum_required(VERSION 3.27)
project(PlayfairCrackerV2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(PlayfairCrackerV2
       Cracker.h
        Cracker.cpp
        main.cpp
        quadgram.h)