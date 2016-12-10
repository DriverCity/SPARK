cmake_minimum_required(VERSION 2.4)

project(BLENode)

# Lacking version information causes CMake warnings.
set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

set(INST_FILES
    device-information-service.js
    main.js
    package.json
    parkCharacteristic.js
    README.md
)

INSTALL(FILES ${INST_FILES} DESTINATION $ENV{DEPLOY_DIR}/bin/${PROJECT_NAME})
INSTALL(DIRECTORY node_modules DESTINATION $ENV{DEPLOY_DIR}/bin/${PROJECT_NAME})
