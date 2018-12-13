cmake_minimum_required(VERSION 3.2)
project(ImageHandler C CXX)

set(SOURCE_FILES
        FileImageHandler.h
        I_ImageHandler.h
        ImageHandlerFactory.h
        IpcamHandler.h
        FileImageHandler.cpp
        ImageHandlerFactory.cpp
        IpcamHandler.cpp
        )

add_library(ImageHandler SHARED STATIC ${SOURCE_FILES})
