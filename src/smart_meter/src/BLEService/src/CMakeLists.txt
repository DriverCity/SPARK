
cmake_minimum_required(VERSION 2.4)

project(BLEService)
add_definitions(-DCOMPONENT_NAME=\\"${PROJECT_NAME}\\")

# Lacking version information causes CMake warnings.
set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

#Get access to other component's public headers.
include_directories($ENV{BUILD_DIR}/include)
include_directories(../include)

set(SRCS BLEService.cpp)

# Link other components
set(LINKED_LIBS Logger)
foreach(lib ${LINKED_LIBS})
    link_directories($ENV{BUILD_DIR}/${lib}/bin)
    message($ENV{BUILD_DIR}/${lib}/bin)
endforeach(lib)

add_library(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} ${LINKED_LIBS})
