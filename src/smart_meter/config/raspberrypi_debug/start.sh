#/bin/bash

BASEDIR=$(dirname "$0")
export LD_LIBRARY_PATH=$BASEDIR/bin
VARIANT=raspberrypi_debug

# stop bluetooth daemon and start bluetooth adapter.
systemctl stop bluetooth
hciconfig hci0 up

node $BASEDIR/bin/BLENode/main.js &
$BASEDIR/bin/application

