cmake_minimum_required(VERSION 3.2)
project(unit_tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D UNIT_TESTS_RUNNING")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${DIVISION_HEADERS_DIR})
include_directories(${IMGDEBUGPRINTER_HEADERS_DIR})
include_directories(${PIPSDETECTOR_HEADERS_DIR})
include_directories(${DOMINOLIB_HEADERS_DIR})
include_directories(${IMGHANDLER_HEADERS_DIR})
include_directories(${IMGCLIPPING_HEADERS_DIR})
include_directories(${GAME_HEADERS_DIR})

file(GLOB IMGDEBUGPRINTER_SOURCES "${IMGDEBUGPRINTER_HEADERS_DIR}/*.cpp")
file(GLOB PIPSDETECTOR_SOURCES "${PIPSDETECTOR_HEADERS_DIR}/*.cpp")
file(GLOB DOMINOLIB_SOURCES "${DOMINOLIB_HEADERS_DIR}/*.cpp")
file(GLOB IMGHANDLER_SOURCES "${IMGHANDLER_HEADERS_DIR}/*.cpp")
file(GLOB IMGCLIPPING_SOURCES "${IMGCLIPPING_HEADERS_DIR}/*.cpp")
file(GLOB GAME_SOURCES "${GAME_HEADERS_DIR}/*.cpp")

set(SOURCE_FILES main.cpp src/divider_tests.cpp ../src/division/division.cpp src/PipDetector_tests.cpp src/DominoCV_tests.cpp src/PlayerPiecesTest.cpp
        "${IMGDEBUGPRINTER_SOURCES}"
        "${PIPSDETECTOR_SOURCES}"
        "${DOMINOLIB_SOURCES}"
        "${IMGHANDLER_SOURCES}"
        "${IMGCLIPPING_SOURCES}"
        "${GAME_SOURCES}")

message("TEST SOURCE FILES: ${SOURCE_FILES}")

add_executable(
        unit_tests
        ${SOURCE_FILES}
)

target_link_libraries(
        unit_tests
        ${OpenCV_LIBS}
        gtest_main
        gmock_main
)

add_test(
        NAME
        unit
        COMMAND
        ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests
)

install(TARGETS unit_tests DESTINATION bin)