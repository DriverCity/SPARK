#/bin/bash

export VARIANT=raspberrypi_release

if [ -d "$PWD/$VARIANT" ]; then
  rm -rf $PWD/$VARIANT
fi
