
cmake_minimum_required(VERSION 2.4)

project(PriceProvider)
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
set(SRCS
  PriceProvider.cpp PriceInfo.cpp
)

set(INST_HDRS
    ../include/IPriceProvider.h
    ../include/PriceProvider.h
    ../include/PriceInfo.h
    ../include/PriceProviderMock.h
)

# List used other components here.
set(LINKED_LIBS CloudService)

# Link other components. Do not change.
foreach(lib ${LINKED_LIBS})
    link_directories($ENV{BUILD_DIR}/${lib}/bin)
endforeach(lib)

# Set output directory for binaries. Do not change.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_library(${PROJECT_NAME} SHARED ${SRCS})

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION $ENV{DEPLOY_DIR}/bin)
INSTALL(FILES ${INST_HDRS} DESTINATION $ENV{BUILD_DIR}/include/${PROJECT_NAME})
