# Driver mobile application

The mobile side of this project is divided into two mobile apps for the (1) drivers and the (2) attendants. This section contains the mobile application implemented for the drivers. The primary objective of the driver app is to store parking events on the cloud by sending useful information to the smart meter (using bluetooth low energy) or the restful web service.

## Functionalities

- **Map** : Real-time map displaying parking areas in the city center of Tampere and their occupancy rate
- **Beacon** : Local storage for save one or several beacons and the vehicle's information associated
- **Park (Disc context)** : Communicate beacon's information to the web service using an http request to create a new parking event
- **Park (Paid context)** : Exchange several information with the smart meter to create a new parking event

## Technologies
This app is an hybrid app developed using [Ionic 1](http://ionicframework.com) and [Angular 1](https://angularjs.org/).

## Usage
The last version is proposed on this repository, and is configured to be functional with our own system. Please follow the instructions under the doc directory to set up your own environment.
