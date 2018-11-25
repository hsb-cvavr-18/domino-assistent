cmake_minimum_required(VERSION 3.2)
project(DominoLib C CXX)

set(SOURCE_FILES
    DominoLib.h
    DominoLib.cpp
)

add_library(DominoLib SHARED STATIC ${SOURCE_FILES})
