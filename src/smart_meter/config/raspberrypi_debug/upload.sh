#/bin/bash

BASEDIR=$(dirname "$0")
VARIANT=raspberrypi_debug
BUILD_TYPE=debug

if [ "$1" == "" ]; then
   echo Usage: upload.sh IP_ADDRESS
   exit 1
else
   echo Connecting to $1
   IP="$1"
fi

ZIP_FILE=$BASEDIR/$VARIANT.zip
INSTALL_SCRIP=$BASEDIR/install_$BUILD_TYPE.sh
UNINSTALL_SCRIP=$BASEDIR/uninstall_$BUILD_TYPE.sh
START_SCRIPT=$BASEDIR/$VARIANT/start.sh

scp $ZIP_FILE $INSTALL_SCRIP $UNINSTALL_SCRIP pi@$IP:/home/pi/
