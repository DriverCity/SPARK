#/bin/bash

BASEDIR=$(dirname "$0")
export LD_LIBRARY_PATH=$BASEDIR/bin
$BASEDIR/bin/application

