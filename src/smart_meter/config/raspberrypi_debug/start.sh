#/bin/bash

export VARIANT=raspberrypi_debug

sudo systemctl stop bluetooth
sudo hciconfig hci0 up

sudo node $PWD/$VARIANT/bin/BLENode/main.js &
$PWD/$VARIANT/bin/application

