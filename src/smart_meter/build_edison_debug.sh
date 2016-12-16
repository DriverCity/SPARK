#!/bin/bash

export BUILD_TYPE="debug"
export BUILD_ROOT=$PWD/builds
export DEPLOY_ROOT=$PWD/deploy
export BUILD_NAME="edison_debug"
export BUILD_VARIANT="variant_edison"
export BUILD_COMPONENTS="edison_components"
export BUILD_DELIVERABLES="edison_debug_deliverables"
export SOURCE_DIR="$PWD/src"
export BUILD_DIR="$PWD/builds/$BUILD_NAME"
export DEPLOY_DIR="$PWD/deploy/$BUILD_NAME"
export CFG_DIR="$PWD/config"

python buildtools/build.py $BUILD_COMPONENTS $BUILD_VARIANT $BUILD_DELIVERABLES $@
