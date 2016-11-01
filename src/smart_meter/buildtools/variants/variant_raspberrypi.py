import os
from common_variants import *

os.environ[CXX] = os.environ['HOME'] + '/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++'
os.environ[CC] = os.environ['HOME'] + '/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-gcc'
