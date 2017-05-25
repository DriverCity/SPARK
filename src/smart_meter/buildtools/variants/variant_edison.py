import os
from common_variants import *

os.environ['PATH'] += '/opt/poky-edison/1.7.3/sysroots/x86_64-pokysdk-linux/usr/bin:/opt/poky-edison/1.7.3/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux'

os.environ[CC] = 'i586-poky-linux-gcc'
os.environ[CFLAGS] += ' -m32 -march=core2 -mtune=core2 -msse3 -mfpmath=sse -mstackrealign -fno-omit-frame-pointer --sysroot=/opt/poky-edison/1.7.3/sysroots/core2-32-poky-linux'

os.environ[CXX] = 'i586-poky-linux-g++'
os.environ[CXXFLAGS] += ' -m32 -march=core2 -mtune=core2 -msse3 -mfpmath=sse -mstackrealign -fno-omit-frame-pointer --sysroot=/opt/poky-edison/1.7.3/sysroots/core2-32-poky-linux'

os.environ['LDFLAGS'] = '-Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed'

print (os.environ[CC])
print (os.environ[CXX])
