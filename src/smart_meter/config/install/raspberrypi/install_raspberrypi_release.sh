#/bin/bash

export VARIANT=raspberrypi_release
export BUILD_TYPE=release

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
