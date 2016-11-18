# 3rd party components
This document describes what 3rd party components are needed and how to install them.

## GTest

1. Create a directory for GTest and change to that directory. Get and build GTest:

  ```sh
  wget https://github.com/google/googletest/archive/release-1.7.0.tar.gz
  tar xf release-1.7.0.tar.gz
  cd googletest-release-1.7.0
  cmake -DBUILD_SHARED_LIBS=ON .
  make
  ```

2. Install include files and binaries to place where the linker can find them:

  ```sh
  sudo cp -a include/gtest /usr/include
  sudo cp -a libgtest_main.so libgtest.so /usr/lib/
  ```

## SQLite3

1. Install sqlite3 to dev env: 

  ```sh
  sudo apt-get install sqlite3 libsqlite3-dev
  ```
  
2. Add source files for cross-compilation:
  - Get amalgamation source package: http://www.sqlite.org/
  - Unzip package.
  - Copy header files to 3rd_party/sqlite3/include (create directory). Copy source files to 3rd_party/sqlite3/src.
  - To view database contents in target system intall sqlite3 there also (optional).
