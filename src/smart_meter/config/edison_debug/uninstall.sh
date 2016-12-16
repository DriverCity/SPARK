#/bin/bash

BASEDIR=$(dirname "$0")
VARIANT=edison_debug

if [ -d "$BASEDIR/$VARIANT" ]; then
  rm -rf $BASEDIR/$VARIANT
fi
