#/bin/bash

BASEDIR=$(dirname "$0")
VARIANT=edison_debug
BUILD_TYPE=debug
IP=$1

ZIP_FILE=$BASEDIR/$VARIANT.zip
INSTALL_SCRIP=$BASEDIR/install_$BUILD_TYPE.sh
UNINSTALL_SCRIP=$(dirname $0)/uninstall_$BUILD_TYPE.sh

scp $ZIP_FILE $INSTALL_SCRIP $UNINSTALL_SCRIP root@$IP:/home/root/
