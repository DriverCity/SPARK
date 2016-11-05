import os
from common_variants import *

os.environ[CC] = '/usr/bin/gcc'
os.environ[CXX] = '/usr/bin/g++'
os.environ["LDFLAGS"] = "-lgtest -lgtest_main"
