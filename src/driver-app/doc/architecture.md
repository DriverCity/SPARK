# Driver app architecture

This document describes driver app architecture on high level. Please refer to code documentation for more detailed description on driver app's components.

## Overview

Driver app consists of several controllers, which manage the different states for the application. The driver app communicate with several third part of the system using following methods:
- **Services** are used to send http requests to several web services. There are two services: (1) ```VehicleSrv```to request information about vehicle and ```CloudSrv``` to interact with the Cloud API.
- **Bluetooth low energy** is used to detected beacons and communicate with the smart meters. Exchanges with the smart meter are based on read and write requests on specific characteristics.
- **AngularFire** is a library used to create syncrhonized array based on Firebase references. All the information retrieved from Firebase use this type of array.

The picture below illustrate the driver app architecture.

![alt text](https://github.com/DriverCity/SPARK/blob/master/src/driver-app/doc/assets/generalArchitecture.png "Smart meter components")