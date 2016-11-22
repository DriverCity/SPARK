#/bin/bash

export VARIANT=edison_debug
export BUILD_TYPE=debug

chmod a+x $PWD/uninstall_$BUILD_TYPE.sh
$PWD/uninstall_$BUILD_TYPE.sh

mkdir $PWD/$VARIANT
cd $PWD/$VARIANT
unzip ../$VARIANT.zip
mkfifo bin/BLEInputFifo
mkfifo bin/BLEResponseFifo
cd ..

chmod a+x $PWD/$VARIANT/bin/application
chmod a+x $PWD/start_$BUILD_TYPE.sh
