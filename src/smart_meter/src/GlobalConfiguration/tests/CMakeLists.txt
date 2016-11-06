cmake_minimum_required(VERSION 2.4)

project(GlobalConfigurationTest)

set(SRC ../src)

include_directories(../include)
include_directories(${SRC})
include_directories($ENV{BUILD_DIR}/include)

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
foreach(lib ${LINKED_LIBS})
    link_directories($ENV{BUILD_DIR}/${lib}/bin)
    message($ENV{BUILD_DIR}/${lib}/bin)
endforeach(lib)

# Do not change
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${TEST_SRCS} ${SRCS})
target_link_libraries(${PROJECT_NAME} gtest ${LINKED_LIBS})
