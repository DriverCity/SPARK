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
  git clone git://github.com/raspberrypi/tools.git
  ```
3. Add cross-compiler to PATH:
  - Open ~/.bashrc file to your editor
  - Add 'export PATH=$PATH:**/path/to/your/**tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin' to the end of the file.
  - Save and close. Run following command to apply changes to PATH:
  ```
  . ~/.bashrc
  (Note '.' at the beginning)
  ```

You now have installed the cross compiler, and can invoke it from commandline. Test the compiler as below. If you are running different Linux distribution as your development environment, you may have to install some extra packages (google error messages to find out which ones).


## Hello World
Let's test compilation with simple Hello World application.

1. Make a C++ application that says 'Hello World' (you sure can do this by yourself).

2. Compile

  ```
  arm-linux-gnueabihf-g++ main.cpp -o HelloWorld
  ```

As a result, there should be an exacutable named 'HelloWorld'. This executable is cross-compiled for Raspberry Pi, and cannot be run on your Linux desktop.


## Upload and run program on Raspberry Pi over SSH
This section describes how to upload and run your program on Raspberry Pi over local area network (LAN)

1. Power up Raspberry Pi and connect it to your router with Ethernet cable.

2. Check Raspberry's LAN ip address from the DHCP client list in your router's control web page (usually written to the router).
Let's assume that the ip-address is 192.168.0.101.

3. Compress your application

  ```
  zip HelloWorld.zip HelloWorld
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

6. The archive containing your program should be located in pi's home directory (/home/pi). Unzip the archive and run the application:

  ```
  unzip HelloWorld.zip
  ./HelloWorld
  ```
  Your application should run without errors.
