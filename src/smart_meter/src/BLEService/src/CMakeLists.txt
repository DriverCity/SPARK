
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

set(SRCS
    BLEService.cpp
    DemoModeBLEService.cpp
)

set(INST_HDRS
    ../include/BLEService.h
    ../include/BLEServiceMock.h
    ../include/IBLEService.h
    ../include/DemoModeBLEService.h
)

# Link other components
set(LINKED_LIBS Logger misc GlobalConfiguration PriceProvider pthread)
link_directories($ENV{DEPLOY_DIR}/bin)

# Set output directories. Do not change.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_library(${PROJECT_NAME} SHARED ${SRCS})
target_link_libraries(${PROJECT_NAME} ${LINKED_LIBS})

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION $ENV{DEPLOY_DIR}/bin)
INSTALL(FILES ${INST_HDRS} DESTINATION $ENV{BUILD_DIR}/include/${PROJECT_NAME})
