
cmake_minimum_required(VERSION 2.4)

project(VerifyParkingTest)
add_definitions(-DCOMPONENT_NAME=\\"${PROJECT_NAME}\\")

# Get access to component-under-test's headers.
include_directories(../include)

#Get access to other component's public headers.
include_directories($ENV{BUILD_DIR}/include)

# List test target's source files here.
set(TEST_SRCS
  ../src/VerifyParking.cpp
)

# List test sources here
set(SRCS
   VerifyParkingTest.cpp
)

set(LINKED_LIBS misc Logger)
link_directories($ENV{DEPLOY_DIR}/bin)

# Do not change
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${TEST_SRCS} ${SRCS})
target_link_libraries(${PROJECT_NAME} ${LINKED_LIBS} libgtest.a libgtest_main.a libgmock.a pthread)
