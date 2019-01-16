cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -pthread")
set(CMAKE_CXX_FLAGS_DEBUG)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(DOMINOASSISTANT_INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(DOMINOASSISTANT_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(DOMINOASSISTANT_INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

set(DIVISION_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/division)
set(IMGDEBUGPRINTER_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/ImgDebugPrinter)
set(PIPSDETECTOR_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/PipsDetector)
set(DOMINOLIB_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/DominoLib)
set(IMGCLIPPING_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/ImageClipping)
set(IMGHANDLER_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/ImageHandler)
set(GAME_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/Game)

include_directories(${DOMINOASSISTANT_INSTALL_INCLUDE_DIR})
#include_directories(${DIVISION_HEADERS_DIR})
#include_directories(${ImgDebugPrinter_HEADERS_DIR})

add_subdirectory(src)

include(cmake/googletest.cmake)
fetch_googletest(
        ${PROJECT_SOURCE_DIR}/cmake
        ${PROJECT_BINARY_DIR}/googletest
)
enable_testing()
add_subdirectory(test)

# file copy idom: https://stackoverflow.com/questions/46995733/how-to-set-cmake-in-order-to-add-txt-files-into-working-directory-as-resource
file(GLOB_RECURSE IMG_FILES "${CMAKE_SOURCE_DIR}/img/*.jpg")
file(COPY ${IMG_FILES} DESTINATION ${CMAKE_BINARY_DIR})
file(GLOB_RECURSE IMG_FILES "${CMAKE_SOURCE_DIR}/srcImg/*.jpg")
file(COPY ${IMG_FILES} DESTINATION ${CMAKE_BINARY_DIR})
