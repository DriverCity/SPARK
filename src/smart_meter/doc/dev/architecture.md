# Smart meter architecture

This document describes smart meter architecture on high level. Refer to Doxygen documentation for more detailed description on
smart meter's components and interfaces between them. 

## Overview

Smart meter consists of smart meter application and BLENode. These two parts are run on an Unix-based operating system, that 
supports using Bluetooth, multitasking (processes and threads) and internet connection. The underlaying hardware naturally needs
to support these same features. In practice, smart meter can be installed and run on any modern Linux distribution. 
Application and BLENode are run os same machine and communicate with each other with Unix FIFOs (First In First Out queue). 
BLENode is responsible for connecting and communicating with Driver Applications using BLE (Bluetooth Low Energy, Bluetooth 4.0). 
BLENode passes received requests to application, which handles the requests. 
The application uses HTTP requests to get price information from Firebase and to register parking events to the cloud. 
BLENode is written in Nodejs because of availibility of free easy-to-use third party libraries for BLE communication. 
Application is written in C++ to gain efficiency, which may be required for cost-efficient embedded system. 
Image below will clearify these relations.

![alt text](https://github.com/DriverCity/SPARK/blob/master/src/smart_meter/doc/dev/SmartMeterOverview.png "Smart meter overview")

Splitting smart meter in two parts and having them to communicate in an inter-process manner proofed useful for development
purposes. The FakeBLETool component in the image refers to a development tool made for testing smart meter application without
an actual Bluetooth connection.


