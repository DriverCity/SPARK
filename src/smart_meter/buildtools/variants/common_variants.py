
import os

CFLAGS = "CFLAGS"
CXXFLAGS = "CXXFLAGS"
CC = "CC"
CXX = "CXX"

os.environ[CFLAGS] = "-Wall -Werror"
os.environ[CXXFLAGS] = os.environ[CFLAGS] + " -std=c++11"
