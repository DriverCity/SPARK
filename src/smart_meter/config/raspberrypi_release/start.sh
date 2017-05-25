#/bin/bash

sudo systemctl stop bluetooth
sudo hciconfig hci0 up

BASEDIR=$(dirname "$0")
export LD_LIBRARY_PATH=$BASEDIR/bin
VARIANT=raspberrypi_release

node $BASEDIR/bin/BLENode/main.js &
$BASEDIR/bin/application

