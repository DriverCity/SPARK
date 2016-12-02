#/bin/bash

export VARIANT=edison_debug

sudo systemctl stop bluetooth
sudo hciconfig hci0 up

sudo node $PWD/$VARIANT/bin/BLENode/main.js &
$PWD/$VARIANT/bin/application
