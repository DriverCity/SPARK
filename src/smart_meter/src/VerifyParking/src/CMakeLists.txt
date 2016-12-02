
cmake_minimum_required(VERSION 2.4)

project(VerifyParking)
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

# List all source files here.
set(SRCS VerifyParking.cpp)

# List used other components here.
set(LINKED_LIBS CloudService ParkingDatabase Logger)


# Link other components. Do not change.
foreach(lib ${LINKED_LIBS})
    link_directories($ENV{BUILD_DIR}/${lib}/bin)
endforeach(lib)

# Linker searches other components from working directory at runtime
# DO NOT CHANGE / REMOVE
SET(CMAKE_EXE_LINKER_FLAGS
          "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,$ORIGIN")

# Set output directory for binaries. Do not change.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Uncomment when there are own sources.
add_library(${PROJECT_NAME} SHARED ${SRCS})
