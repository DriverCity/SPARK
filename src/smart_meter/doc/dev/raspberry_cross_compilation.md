# Cross-compiling for Raspberry Pi on Linux

This document tells how to cross-compile C++ application for Raspberry Pi on Linux desktop.

## First time setup
Following steps need to be executed only once before first cross-compilation.

1. install required packages:
   ```
   sudo apt-get install git cmake rsync lib32ncurses5
   ```

2. Get cross-compiler for Raspberry Pi:
  ```
  mkdir ~/raspberrypi
  cd ~/raspberrypi
  git clone git://github.com/raspberrypi/tools.git
  ```
3. Add cross-compiler to PATH:
  - Open ~/.bashrc file to your editor
  - Add 'export PATH=$PATH:$HOME/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin' to the end of the file.
  - Save and close. Run following command to apply changes to PATH:
  ```
  . ~/.bashrc
  (Note '.' at the beginning)
  ```

You now have installed the cross compiler, and can invoke it from commandline.


## Hello World
Let's test compilation with simple Hello World application.

1. Make directory for your project. Go to that directory.
2. Add CMakeLists.txt with following contents:
  ```cmake
  CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
  project(HelloWorld)
  set(VERSION_MAJOR "1")
  set(VERSION_MINOR "0")
  set(VERSION_PATCH "0")
  set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

  set(SRC main.cpp)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_CXX_COMPILER $ENV{HOME}/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++)

  add_executable(${PROJECT_NAME} ${SRC})
  ```
  
3. Create main.cpp containing the source code (you sure can do this by yourself).
4. Generate makefile and compile

  ```
  cmake ./
  make
  ```

As a result, there should be a new directory in your source directory called 'bin'. 
This directory contains executable 'HelloWorld'. 
This executable is cross-compiled for Raspberry Pi, and cannot be run on your Linux desktop.


## Upload and run program on Raspberry Pi over SSH
This section describes how to upload and run your program on Raspberry Pi over local area network (LAN)

1. Power up Raspberry Pi and connect it to your router with Ethernet cable.

2. Check Raspberry's LAN ip address from the DHCP client list in your router's control web page (usually written to the router).
Let's assume that the ip-address is 192.168.0.101.

3. Compress your application

  ```
  cd bin
  zip -r HelloWorld.zip ./
  ```
  
4. Send archive to Raspberry Pi.
  
  ```
  scp HelloWorld.zip pi@192.168.0.101:/home/pi
  ```
  On first time accessing Raspberry Pi, you may first need to add target ip to known hosts:
  ```
  ssh-keygen -f "$HOME/.ssh/known_hosts" -R 192.168.0.101
  ```
  You also may be asked confirmation while running scp. Default password for pi is 'raspberry'.
  
5. Login to Raspberry Pi over SSH. Use SSH client of your choise (e.g. Putty). Login name is 'pi' and password is 'raspberry'.

6. The archive containing your program should be located in pi's home directory (/home/pi).

7. Unzip the archive and run the application:

  ```
  unzip HelloWorld.zip
  ./HelloWorld
  ```
  Your application should run without errors.
