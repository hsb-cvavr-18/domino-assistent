cmake_minimum_required(VERSION 3.2)
project(PipsDetector C CXX)

set(SOURCE_FILES
    PipsDetector.h
    PipsDetector.cpp
)

add_library(PipsDetector SHARED STATIC ${SOURCE_FILES})
