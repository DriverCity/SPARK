#/bin/bash

BASEDIR=$(dirname "$0")
VARIANT=edison_debug
BUILD_TYPE=debug

chmod a+x $PWD/uninstall_$BUILD_TYPE.sh
$BASEDIR/uninstall_$BUILD_TYPE.sh

mkdir $BASEDIR/$VARIANT
cd $BASEDIR/$VARIANT
unzip $BASEDIR/$VARIANT.zip
mkfifo $BASEDIR/bin/BLEInputFifo
mkfifo $BASEDIR/bin/BLEResponseFifo
cd $BASEDIR

chmod a+x $BASEDIR/$VARIANT/bin/application
chmod a+x $BASEDIR/$VARIANT/bin/FakeBLETool
chmod a+x $BASEDIR/$VARIANT/start_$BUILD_TYPE.sh

cd $VARIANT/$VARIANT/bin/BLENode
npm install npm install bluetooth-hci-socket
