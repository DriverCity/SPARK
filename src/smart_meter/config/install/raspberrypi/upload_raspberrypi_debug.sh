#/bin/bash

export VARIANT=raspberrypi_debug
export BUILD_TYPE=debug

if [ "$1" == "" ]; then
   echo Using default ip 169.254.186.133
   export IP="169.254.186.133"
else
   echo Connecting to $1
   export IP="$1"
fi

export ZIP_FILE=$(dirname $0)/$VARIANT.zip
export INSTALL_SCRIP=$(dirname $0)/install_$BUILD_TYPE.sh
export UNINSTALL_SCRIP=$(dirname $0)/uninstall_$BUILD_TYPE.sh
export START_SCRIPT=$(dirname $0)/start_$BUILD_TYPE.sh

scp $ZIP_FILE $INSTALL_SCRIP $UNINSTALL_SCRIP $START_SCRIPT pi@$IP:/home/pi/
