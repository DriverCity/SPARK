cmake_minimum_required(VERSION 2.4)

project(jsoncpp)

# Lacking version information causes CMake warnings.
set(VERSION_MAJOR "1")
set(VERSION_MINOR "8")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

include_directories(../include)
link_directories($ENV{COMPILER_LIBS})

set(SRCS jsoncpp.cpp)

set(INST_HDRS
    json/json.h
    json/json-forwards.h)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_library(${PROJECT_NAME} SHARED ${SRCS})
target_link_libraries(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION $ENV{DEPLOY_DIR}/bin)
install(FILES ${INST_HDRS} DESTINATION $ENV{BUILD_DIR}/include/json)
