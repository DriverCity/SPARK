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

## Smart meter application

This section gives broad description on application part's architecture. 
Refer to doxygen documentation for more detailed description.

Smart meter application consists of multiple components communicating with each other through abstract interfaces. Image below shows the most significant components and relations between them.

![alt text](https://github.com/DriverCity/SPARK/blob/master/src/smart_meter/doc/dev/SmartMeterComponents.png "Smart meter components")

Here is a brief description of each of these components. Refer to doxygen documentation for detailed description for these and for the rest of the components.

- **Application** creates and initializes other components. Application is also responsible for starting and quiting BLEService when signalled to do so.

- **BLEService** listens for incomming messages from BLENode and responds to them. It uses PriceProvider and VerifyParking to carry out requested actions.

- **PriceProvider** is a proxy between BLEService and  CloudService. BLEService uses it to get price information for the area smart meter is deployed. Currently PriceProvider simply passes the request forward to CloudService. More functionality may be added at later stage.

- **VerifyParking** is a proxy between BLEService and CloudService. BLEService uses it to register a parking event to the cloud. Currently VerifyParking simply passes the request forward to CloudService. More functionality may be added at later stage.

- **CloudService** comminicates with the cloud. It sends HTTP-requests to fetch parking price information from Firebase and to register parking events to the cloud. CloudService uses the curl library to handle http-requests.

## Doxygen documentation

All smart meter's modules have been documented in comments in the code. 
You may generate a more readable documentation from these comments using Doxygen. 
Install doxygen on your development environment, change to the smart_meter/doc/dev directory and generate 
documentation running command **doxygen** in said directory. This will generate html-documents in html subdirectory.
