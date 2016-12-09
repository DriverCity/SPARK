#/bin/bash

export VARIANT=edison_debug
export BUILD_TYPE=debug
export IP=$1

export ZIP_FILE=$(dirname $0)/$VARIANT.zip
export INSTALL_SCRIP=$(dirname $0)/install_$BUILD_TYPE.sh
export UNINSTALL_SCRIP=$(dirname $0)/uninstall_$BUILD_TYPE.sh
export START_SCRIPT=$(dirname $0)/start_$BUILD_TYPE.sh

scp $ZIP_FILE $INSTALL_SCRIP $UNINSTALL_SCRIP $START_SCRIPT root@$IP:/home/root/
