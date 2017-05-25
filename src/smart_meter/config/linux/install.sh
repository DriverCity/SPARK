#/bin/bash

BASEDIR=$(dirname "$0")
mkfifo $BASEDIR/bin/BLEInputFifo
mkfifo $BASEDIR/bin/BLEResponseFifo
