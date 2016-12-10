#/bin/bash

BASEDIR=$(dirname "$0")
VARIANT=raspberrypi_debug
BUILD_TYPE=debug

chmod a+x $BASEDIR/uninstall_$BUILD_TYPE.sh
$BASEDIR/uninstall_$BUILD_TYPE.sh

mkdir $BASEDIR/$VARIANT
cd $BASEDIR/$VARIANT
unzip ../$VARIANT.zip
mkfifo bin/BLEInputFifo
mkfifo bin/BLEResponseFifo
cd ..

chmod a+x $BASEDIR/$VARIANT/bin/application
chmod a+x $BASEDIR/$VARIANT/start.sh

cd $BASEDIR/$VARIANT/bin/BLENode
npm install bluetooth-hci-socket
