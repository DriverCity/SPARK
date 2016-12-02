
cmake_minimum_required(VERSION 2.4)

project(PriceProviderTest)
add_definitions(-DCOMPONENT_NAME=\\"${PROJECT_NAME}\\")

# Get access to component-under-test's headers.
include_directories(../include)

#Get access to other component's public headers.
include_directories($ENV{BUILD_DIR}/include)

# List test target's source files here.
set(TEST_SRCS
  ../src/PriceInfo.cpp
  ../src/PriceProvider.cpp
)

# List test sources here
set(SRCS
   PriceInfoTest.cpp
   PriceProviderTest.cpp
)

set(LINKED_LIBS misc)
foreach(lib ${LINKED_LIBS})
    link_directories($ENV{BUILD_DIR}/${lib}/bin)
    message($ENV{BUILD_DIR}/${lib}/bin)
endforeach(lib)

# Do not change
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${TEST_SRCS} ${SRCS})
target_link_libraries(${PROJECT_NAME} ${LINKED_LIBS} gtest)
