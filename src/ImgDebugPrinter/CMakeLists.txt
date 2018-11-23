cmake_minimum_required(VERSION 3.2)
project(ImgDebugPrinter C CXX)

set(SOURCE_FILES
        ImgDebugPrinter.h
        StubImgDebugPrinter.cpp
        DefaultImgDebugPrinter.cpp
        )

add_library(ImgDebugPrinter SHARED STATIC ${SOURCE_FILES})
