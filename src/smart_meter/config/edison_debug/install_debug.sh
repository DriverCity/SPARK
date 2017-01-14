#/bin/bash

BASEDIR=$(dirname "$0")
VARIANT=edison_debug
BUILD_TYPE=debug

chmod a+x $PWD/uninstall_$BUILD_TYPE.sh
$BASEDIR/uninstall_$BUILD_TYPE.sh

mkdir $BASEDIR/$VARIANT
cd $BASEDIR/$VARIANT
unzip ../$VARIANT.zip
mkfifo bin/BLEInputFifo
mkfifo bin/BLEResponseFifo
chmod a+x bin/application
chmod a+x bin/FakeBLETool
chmod a+x start.sh

cd bin/BLENode
npm install
