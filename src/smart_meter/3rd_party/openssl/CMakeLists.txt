cmake_minimum_required(VERSION 2.4)

project(openssl)

# Lacking version information causes CMake warnings.
set(VERSION_MAJOR "7")
set(VERSION_MINOR "52")
set(VERSION_PATCH "1")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

install(FILES $ENV{OPENSSL_DIR}/lib/libssl.so $ENV{OPENSSL_DIR}/lib/libssl.so.1.1 DESTINATION $ENV{DEPLOY_DIR}/bin)
install(FILES $ENV{OPENSSL_DIR}/lib/libcrypto.so $ENV{OPENSSL_DIR}/lib/libcrypto.so.1.1 DESTINATION $ENV{DEPLOY_DIR}/bin)
install(DIRECTORY $ENV{OPENSSL_DIR}/include/openssl DESTINATION $ENV{BUILD_DIR}/include)
