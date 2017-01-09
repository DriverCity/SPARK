
import sys
from variant_linux import *

os.environ[CFLAGS] += ' -O0 --coverage'
os.environ[CXXFLAGS] += ' -O0 --coverage'
