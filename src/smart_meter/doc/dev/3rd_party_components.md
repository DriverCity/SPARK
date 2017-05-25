# 3rd party components
This document describes what 3rd party components are needed and how to install them.

## GTest and GMock

1. Create a directory for GTest and change to that directory. Get and build GTest:

  ```sh
  wget https://github.com/google/googletest/archive/release-1.8.0.zip
  unzip release-1.8.0.zip
  mkdir googletest-release-1.8.0/build
  cd googletest-release-1.8.0/build
  cmake ..
  make
  cmake -DBUILD_SHARED_LIBS=ON ..
  make
  ```

2. Install include files and binaries to place where the linker can find them:

  ```sh
  sudo cp -a ../googletest/include/gtest /usr/include
  sudo cp -a ../googlemock/include/gmock /usr/include
  sudo cp -a googlemock/gtest/libgtest_main.so googlemock/gtest/libgtest.so /usr/lib/
  sudo cp -a googlemock/gtest/libgtest_main.a googlemock/gtest/libgtest.a /usr/lib/
  sudo cp -a googlemock/libgmock_main.so googlemock/libgmock.so /usr/lib/
  sudo cp -a googlemock/libgmock_main.a googlemock/libgmock.a /usr/lib/
  ```

## SQLite3

Install sqlite3 to dev env: 

```sh
sudo apt-get install sqlite3 libsqlite3-dev
```

## Openssl

This is only needed for Raspberry Pi.

- Cross-compile openssl for Raspberry Pi:

  ```sh
  git clone https://github.com/openssl/openssl.git
  cd openssl
  ./Configure linux-generic32 shared --prefix=$PWD/build --openssldir=$PWD/build/openssl --cross-compile-prefix=arm-linux-gnueabihf-
  make depend && make && make install
  ```

- Add following line to ~/.bashrc and restart terminal to apply.
  ```sh
  export OPENSSL_DIR=**/path/to/ssl**/build
  ```

## Curl

Curl is included in Edison development kit. No installation is required if you are building for Intel Edison.

Install libcurl package for development environment:

```sh
sudo apt-get install libcurl4-openssl-dev
```

Cross-compile curl for Raspberry Pi:

- Make a directory to location of your choise. Run following commands to cross-compile curl for ARM.

  ```sh
  wget https://curl.haxx.se/download/curl-7.52.1.tar.gz
  tar -xf curl-7.52.1.tar.gz
  cd curl-7.52.1
  mkdir build && mkdir build/lib && cd build/lib
  ln -s $OPENSSL_DIR/lib/libssl.so.1.1
  ln -s $OPENSSL_DIR/lib/libcrypto.so.1.1
  cd ../..
  ./configure --host=arm-linux-gnueabihf --prefix=$PWD/build --with-ssl=$OPENSSL_DIR
  make && make install
  ```
  
- In ~/.bashrc, insert following line and restart terminal to apply changes.
   ```sh
   export CURL_DIR=/path/to/your/curl-7.52.1/build
   ```
 
 
