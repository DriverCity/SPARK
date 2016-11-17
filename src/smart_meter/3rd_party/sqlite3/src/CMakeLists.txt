cmake_minimum_required(VERSION 2.4)

project(sqlite3)

# Lacking version information causes CMake warnings.
set(VERSION_MAJOR "3")
set(VERSION_MINOR "15")
set(VERSION_PATCH "1")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

include_directories(../include)
link_directories($ENV{COMPILER_LIBS})

set(SRCS sqlite3.c)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_library(${PROJECT_NAME} SHARED ${SRCS})
target_link_libraries(${PROJECT_NAME} dl pthread)
