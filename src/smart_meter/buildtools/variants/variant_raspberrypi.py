import os
from common_variants import *

os.environ[CXX] = os.environ['HOME'] + '/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++'
os.environ[CC] = os.environ['HOME'] + '/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-gcc'

os.environ[CFLAGS] += ' --sysroot /mnt/raspberry-rootfs'
os.environ[CXXFLAGS] += ' --sysroot /mnt/raspberry-rootfs'

# Switch optimitisation on for release build.
if os.environ['BUILD_TYPE'] == "release":
   os.environ[CFLAGS] += " -O3"
   os.environ[CXXFLAGS] += " -O3"

else:
    os.environ[CFLAGS] += " -O0"
    os.environ[CXXFLAGS] += " -O0"
