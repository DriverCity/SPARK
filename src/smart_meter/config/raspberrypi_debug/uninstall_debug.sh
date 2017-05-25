#/bin/bash

BASEDIR=$(dirname "$0")
VARIANT=raspberrypi_debug

if [ -d "$BASEDIR/$VARIANT" ]; then
  rm -rf $BASEDIR/$VARIANT
fi
