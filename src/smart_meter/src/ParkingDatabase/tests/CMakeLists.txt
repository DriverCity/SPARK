cmake_minimum_required(VERSION 2.4)

project(ParkingDatabaseTest)
add_definitions(-DCOMPONENT_NAME=\\"${PROJECT_NAME}\\")

# Get access to component-under-test's headers.
include_directories(../include)
include_directories($ENV{BUILD_DIR}/include)

# List test target's source files here.
set(TEST_SRCS
  ../src/ParkingDatabase.cpp
)

# List test sources here
set(SRCS
  ParkingDatabaseTest.cpp
)

# Link other components
set(LINKED_LIBS Logger misc)
foreach(lib ${LINKED_LIBS})
    link_directories($ENV{BUILD_DIR}/${lib}/bin)
endforeach(lib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${TEST_SRCS} ${SRCS})
target_link_libraries(${PROJECT_NAME} gtest gtest_main sqlite3 ${LINKED_LIBS})
