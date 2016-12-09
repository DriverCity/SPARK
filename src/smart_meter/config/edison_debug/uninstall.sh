#/bin/bash

export VARIANT=edison_debug

if [ -d "$PWD/$VARIANT" ]; then
  rm -rf $PWD/$VARIANT
fi
