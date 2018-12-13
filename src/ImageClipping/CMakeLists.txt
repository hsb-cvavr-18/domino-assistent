cmake_minimum_required(VERSION 3.2)
project(ImageClipping C CXX)

set(SOURCE_FILES
        ImageClipping.h
        ImageClipping.cpp
        )

add_library(ImageClipping SHARED STATIC ${SOURCE_FILES})
