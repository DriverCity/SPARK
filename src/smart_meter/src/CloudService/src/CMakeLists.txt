
cmake_minimum_required(VERSION 2.4)

project(CloudService)
add_definitions(-DCOMPONENT_NAME=\\"${PROJECT_NAME}\\")

# Lacking version information causes CMake warnings.
set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

include_directories($ENV{BUILD_DIR}/include)
include_directories(../include)

set(SRCS CloudService.cpp)

# List used other components here.
set(LINKED_LIBS misc)

set(INST_HDRS
    ../include/CloudServiceMock.h
    ../include/ICloudService.h
    ../include/CloudService.h
)

# Link other components.
foreach(lib ${LINKED_LIBS})
    link_directories($ENV{BUILD_DIR}/${lib}/bin)
endforeach(lib)

# Set output directory for binaries. Do not change.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_library(${PROJECT_NAME} SHARED ${SRCS})
target_link_libraries(${PROJECT_NAME} curl)

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION $ENV{DEPLOY_DIR}/bin)
INSTALL(FILES ${INST_HDRS} DESTINATION $ENV{BUILD_DIR}/include/${PROJECT_NAME})
