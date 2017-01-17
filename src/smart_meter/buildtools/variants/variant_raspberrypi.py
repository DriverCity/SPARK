import os
from common_variants import *

os.environ[CXX] = 'arm-linux-gnueabihf-g++'
os.environ[CC] = 'arm-linux-gnueabihf-gcc'

# Switch optimitisation on for release build.
if os.environ['BUILD_TYPE'] == "release":
   os.environ[CFLAGS] += " -O3"
   os.environ[CXXFLAGS] += " -O3"

else:
    os.environ[CFLAGS] += " -O0"
    os.environ[CXXFLAGS] += " -O0"
