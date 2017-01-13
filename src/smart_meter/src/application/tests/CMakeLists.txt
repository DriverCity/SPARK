cmake_minimum_required(VERSION 2.4)

project(ApplicationTest)
add_definitions(-DCOMPONENT_NAME=\\"${PROJECT_NAME}\\")

include_directories(../src)
include_directories($ENV{BUILD_DIR}/include)

add_definitions(-DTEST_DATA_DIR=\\"${CMAKE_CURRENT_SOURCE_DIR}/data\\")

set(TEST_SRCS
  ../src/Initializer.cpp
  ../src/Application.cpp
)

# List test sources here
set(SRCS
    InitializerTest.cpp
    ApplicationTest.cpp
)

# Link other components
set(LINKED_LIBS
    Logger
    GlobalConfiguration
    ParkingDatabase
    misc
    BLEService
    PriceProvider
    CloudService
    VerifyParking
)
foreach(lib ${LINKED_LIBS})
    link_directories($ENV{BUILD_DIR}/${lib}/bin)
endforeach(lib)

# Do not change
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${TEST_SRCS} ${SRCS})
target_link_libraries(${PROJECT_NAME} ${LINKED_LIBS} libgtest.a libgmock.a libgtest_main.a pthread)
