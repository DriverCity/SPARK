#/bin/bash

export VARIANT=raspberrypi_debug

if [ -d "$PWD/$VARIANT" ]; then
  rm -rf $PWD/$VARIANT
fi
