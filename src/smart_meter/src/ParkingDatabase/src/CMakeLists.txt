cmake_minimum_required(VERSION 2.4)

project(ParkingDatabase)
add_definitions(-DCOMPONENT_NAME=\\"${PROJECT_NAME}\\")

# Lacking version information causes CMake warnings. 
set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# Get access to component's own headers
include_directories(../include)

#Get access to other component's public headers.
include_directories($ENV{BUILD_DIR}/include)

if(NOT ($ENV{BUILD_NAME} STREQUAL "linux_debug"))
    include_directories($ENV{BUILD_DIR}/include/sqlite3)
endif()

# List all source files here.
set(SRCS ParkingDatabase.cpp)

set(INST_HDRS
    ../include/IParkingDatabase.h
    ../include/ParkingDatabase.h
    ../include/ParkingDatabaseMock.h
)

set(LINKED_LIBS sqlite3 Logger misc)
link_directories($ENV{DEPLOY_DIR}/bin)

# Set output directory for binaries. Do not change.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_library(${PROJECT_NAME} SHARED ${SRCS})
target_link_libraries(${PROJECT_NAME} ${LINKED_LIBS})

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION $ENV{DEPLOY_DIR}/bin)
INSTALL(FILES ${INST_HDRS} DESTINATION $ENV{BUILD_DIR}/include/${PROJECT_NAME})
