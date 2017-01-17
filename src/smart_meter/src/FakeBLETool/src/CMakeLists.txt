cmake_minimum_required(VERSION 2.4)

# Component name (should match source directory name)
project(FakeBLETool)
add_definitions(-DCOMPONENT_NAME=\\"${PROJECT_NAME}\\")

set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# Set output directories. Do not change.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Access other component's public headers. Do not change.
include_directories($ENV{BUILD_DIR}/include)

# Define all sources here
set(SRCS main.cpp)

# Link other components
set(LINKED_LIBS Logger GlobalConfiguration)
link_directories($ENV{DEPLOY_DIR}/bin)

add_executable(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} ${LINKED_LIBS})

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION $ENV{DEPLOY_DIR}/bin)
