
cmake_minimum_required(VERSION 2.4)

project(misctest)
add_definitions(-DCOMPONENT_NAME=\\"${PROJECT_NAME}\\")

# Get access to component-under-test's headers.
include_directories(../include)

# List test target's source files here.
set(TEST_SRCS
  ../src/ParkingEvent.cpp
)

# List test sources here
set(SRCS
  ParkingEventTest.cpp
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${TEST_SRCS} ${SRCS})
target_link_libraries(${PROJECT_NAME} gtest)
