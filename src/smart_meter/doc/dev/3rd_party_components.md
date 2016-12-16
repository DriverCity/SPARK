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


## Google APIs C++ client

1. Download source code from GoogleDrive (googleapis.zip in the root). Make a directory and unzip archive there.

2. Run following commands to build and install Google APIs

  ```sh
  # Build dependencies
  python ./prepare_dependencies.py
  
  # Build Google APIs
  mkdir build && cd build
  cmake ..
  make
  make install
  
  # make headers available
  cd ..
  sudo cp -a build/include/googleapis /usr/include
  sudo cp -a external_dependencies/install/gflags /usr/include
  sudo cp -a external_dependencies/install/glog /usr/include
  sudo cp -a external_dependencies/install/google /usr/include
  sudo cp -a external_dependencies/install/json /usr/include
  sudo cp -a external_dependencies/install/openssl /usr/include
  sudo cp -a external_dependencies/install/curl /usr/include
  
  # make binaries available
  sudo cp build/lib/* /usr/lib/
  sudo cp external_dependencies/install/lib/* /usr/lib/
  ```
  
  
