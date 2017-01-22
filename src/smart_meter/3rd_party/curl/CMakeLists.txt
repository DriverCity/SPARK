cmake_minimum_required(VERSION 2.4)

project(jsoncpp)

# Lacking version information causes CMake warnings.
set(VERSION_MAJOR "7")
set(VERSION_MINOR "52")
set(VERSION_PATCH "1")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

install(FILES $ENV{CURL_DIR}/lib/libcurl.so $ENV{CURL_DIR}/lib/libcurl.so.4 $ENV{CURL_DIR}/lib/libcurl.so.4.4.0 DESTINATION $ENV{DEPLOY_DIR}/bin)
install(DIRECTORY $ENV{CURL_DIR}/include/curl DESTINATION $ENV{BUILD_DIR}/include)
