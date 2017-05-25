cmake_minimum_required(VERSION 2.4)

project(GlobalConfigurationTest)
add_definitions(-DCOMPONENT_NAME=\\"${PROJECT_NAME}\\")

set(SRC ../src)

include_directories(../include)
include_directories($ENV{BUILD_DIR}/include)

add_definitions(-DTEST_DATA_DIR=\\"${CMAKE_CURRENT_SOURCE_DIR}/data\\")

set(TEST_SRCS
  ${SRC}/ConfigurationReader.cpp
  ${SRC}/GlobalConfiguration.cpp
)

# List test sources here
set(SRCS
  ConfigurationReaderTest.cpp
  GlobalConfigurationTest.cpp
)

# Link other components
set(LINKED_LIBS Logger)
link_directories($ENV{DEPLOY_DIR}/bin)

# Do not change
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${TEST_SRCS} ${SRCS})
target_link_libraries(${PROJECT_NAME} gtest gtest_main ${LINKED_LIBS})
