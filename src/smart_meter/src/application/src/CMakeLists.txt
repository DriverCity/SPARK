cmake_minimum_required(VERSION 2.4)

# Component name (should match source directory name)
project(application)
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
set(SRCS main.cpp Application.cpp)

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

# Linker searches other components from working directory at runtime
SET(CMAKE_EXE_LINKER_FLAGS
          "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,$ORIGIN")


add_executable(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} ${LINKED_LIBS})
