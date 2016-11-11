import os
from common_variants import *

os.environ[CC] = "i586-poky-linux-gcc -m32 -march=core2 -mtune=core2 -msse3 -mfpmath=sse -mstackrealign -fno-omit-frame-pointer --sysroot=/opt/poky-edison/1.7.3/sysroots/core2-32-poky-linux"

os.environ[CXX] = "i586-poky-linux-g++ -m32 -march=core2 -mtune=core2 -msse3 -mfpmath=sse -mstackrealign -fno-omit-frame-pointer --sysroot=/opt/poky-edison/1.7.3/sysroots/core2-32-poky-linux"

