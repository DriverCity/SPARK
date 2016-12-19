#/bin/bash

BASEDIR=$(dirname "$0")
VARIANT=raspberrypi_release

if [ -d "$BASEDIR/$VARIANT" ]; then
  rm -rf $BASEDIR/$VARIANT
fi
