#/bin/bash

BASEDIR=$(dirname "$0")
VARIANT=raspberrypi_debug

if [ "$1" = "clean" ]; then
    # Install dependencies
    wget https://nodejs.org/dist/v5.9.1/node-v5.9.1-linux-armv7l.tar.xz
    tar xfv node-v5.9.1-linux-armv7l.tar.xz
    cd node-v5.9.1-linux-armv7l/
    sudo cp -R * /usr/local/
    cd ..
    rm -r node-v5.9.1-linux-armv7l
    rm node-v5.9.1-linux-armv7l.tar.xz
    mkdir $BASEDIR/$VARIANT

elif [ "$1" != "upgrade" ]; then
    echo "usage:"
    echo "'install_debug.sh clean' for clean installation."
    echo "'install_debug.sh upgrade' to upgrade existing software."
    exit 1
fi

cd $BASEDIR/$VARIANT
unzip ../$VARIANT.zip
cd ..

chmod a+x $BASEDIR/$VARIANT/bin/application
chmod a+x $BASEDIR/$VARIANT/bin/FakeBLETool
chmod a+x $BASEDIR/$VARIANT/start.sh

if [ $1 = "clean" ]; then
    # Create fifos and install nodejs modules.
    mkfifo $VARIANT/bin/BLEInputFifo
    mkfifo $VARIANT/bin/BLEResponseFifo
    cd $BASEDIR/$VARIANT/bin/BLENode
    npm install
fi

echo Installation completed successfully!
