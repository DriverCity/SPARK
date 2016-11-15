cmake_minimum_required(VERSION 2.4)

project(GlobalConfiguration)
add_definitions(-DCOMPONENT_NAME=\\"${PROJECT_NAME}\\")

set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

include_directories(../include)
include_directories($ENV{BUILD_DIR}/include)

# List all source files here.
set(SRCS
   ConfigurationReader.cpp
   GlobalConfiguration.cpp
)

# Link other components
set(LINKED_LIBS Logger)
foreach(lib ${LINKED_LIBS})
    link_directories($ENV{BUILD_DIR}/${lib}/bin)
endforeach(lib)

# DO NOT CHANGE / REMOVE
SET(CMAKE_EXE_LINKER_FLAGS
          "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,$ORIGIN")

# Set output directory for binaries. Do not change.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_library(${PROJECT_NAME} SHARED ${SRCS})