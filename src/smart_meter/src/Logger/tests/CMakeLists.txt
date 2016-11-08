cmake_minimum_required(VERSION 2.4)

project(LoggerTest)
add_definitions(-DCOMPONENT_NAME=\\"${PROJECT_NAME}\\")

include_directories(../include)

set(TEST_SRCS 
  ../src/Logger.cpp
)

# List test sources here
set(SRCS 
  LoggerTest.cpp
)

# Do not change
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${TEST_SRCS} ${SRCS})
target_link_libraries(${PROJECT_NAME} gtest)
