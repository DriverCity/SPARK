#/bin/bash

export BUILD_TYPE="DEBUG"
export BUILD_ROOT=$PWD/builds
export DEPLOY_ROOT=$PWD/deploy
export BUILD_NAME="raspberrypi_debug"
export BUILD_VARIANT="variant_raspberrypi"
export BUILD_COMPONENTS="raspberrypi_components"
export SOURCE_DIR="$PWD/src"
export BUILD_DIR="$PWD/builds/$BUILD_NAME"
export DEPLOY_DIR="$PWD/deploy/$BUILD_NAME"

python buildtools/build.py $BUILD_COMPONENTS $BUILD_VARIANT $@
