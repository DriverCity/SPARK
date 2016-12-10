cmake_minimum_required(VERSION 2.4)

project(BLEService)

# Lacking version information causes CMake warnings.
set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

set(CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../config)

set(CONF_FILES ${CONFIG_DIR}/global.txt)

message("Variant: " $ENV{BUILD_NAME})

if( $ENV{BUILD_NAME} STREQUAL "linux_debug" )
    set(SCRIPT_FILES
        ${CONFIG_DIR}/linux/install.sh
        ${CONFIG_DIR}/linux/start.sh
        ${CONFIG_DIR}/linux/FakeBLETool.sh
    )

else()
    set(SCRIPT_FILES
        ${CONFIG_DIR}/$ENV{BUILD_NAME}/install.sh
        ${CONFIG_DIR}/$ENV{BUILD_NAME}/uninstall.sh
        ${CONFIG_DIR}/$ENV{BUILD_NAME}/start.sh
        ${CONFIG_DIR}/$ENV{BUILD_NAME}/upload.sh
        ${CONFIG_DIR}/linux/FakeBLETool.sh
    )

endif()


install(FILES ${CONF_FILES}     DESTINATION $ENV{DEPLOY_DIR}/config)
install(FILES ${SCRIPT_FILES}   DESTINATION $ENV{DEPLOY_DIR} PERMISSIONS OWNER_READ OWNER_EXECUTE)
