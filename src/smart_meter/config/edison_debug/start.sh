#/bin/bash

VARIANT=edison_debug
BASEDIR=$(dirname "$0")
export LD_LIBRARY_PATH=$BASEDIR/bin

sudo systemctl stop bluetooth
sudo hciconfig hci0 up

sudo node $BASEDIR/bin/BLENode/main.js &
$BASEDIR/bin/application
