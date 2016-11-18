# Development environment setup

This document gives instructions on how to set up your development environment for smart meter.
Developers are recommended to use this setup. HW lead may not be able to help with other setups.

Recommended operating system is Ubuntu 14.04 Trusty. You may install the OS on your computer, or 
run the OS on virtual machine, as Instructed here. If you already have the OS installed, go to phase 3.

1. Install VMWare Workstation 12 Player (non-commercial licence is sufficient for our project)
  
  You may download the software for Windows from here: http://www.vmware.com/products/workstation.html
  
2. Install Ubuntu on virtual machine. 
  
  Get live-CD for Ubuntu from: http://releases.ubuntu.com/14.04/
  Installing 64-bit version may require you to enable hardware virtualization support from BIOS. Refer to your
  PC manufacturer's manual for exact instructions.
  
3. Install required packages  
  
  Run following command to install required packages:
  ```
  sudo apt-get install git python cmake rsync lib32ncurses5
  ```
  
4. Install cross-compiler for Raspberry Pi 3.
  
  Install cross-compilers for intended target devices:
  - [Raspberry Pi 3](https://github.com/DriverCity/SPARK/blob/master/src/smart_meter/doc/dev/raspberry_cross_compilation.md)
  - [Intel Edison](https://github.com/DriverCity/SPARK/blob/master/src/smart_meter/doc/dev/edison_cross_compilation.md)

  
5. Install 3rd party components

  Follow instructions in [3rd_party_components.md.](https://github.com/DriverCity/SPARK/blob/master/src/smart_meter/doc/dev/3rd_party_components.md)


After these steps, you are good to go. Clone SPARK repository to location of your choise 
and see [build_test_and_deploy.md](https://github.com/DriverCity/SPARK/blob/master/src/smart_meter/doc/dev/build_test_and_deploy.md) 
for instructions on how to build the project and add new
components. You may use QtCreator as IDE. Create qt.pro running QtCreatorProject.sh at the 
root of smart meter directory. Note that you cannot build the project from QtCreator.
