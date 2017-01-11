# Attendant mobile application

The mobile side of this project is divided into two mobile apps for the (1) drivers and the (2) attendants.
This section contains the mobile application implemented for the attendants.
The primary objective of the attendant app is to check parking validity for beacons detected using bluetooth low energy by querying the database.

## Functionalities

- **Map** : Real-time map displaying parking areas in the city center of Tampere and their occupancy rate
- **Check (Disc context)** : Set up a time limitation and define beacons' validity if their are stored in disc context
- **Check (Paid context)** : Define beacons' validity based on their arrival time and parking duration stored in the database 

## Technologies
This app is an hybrid app developed using [Ionic 1](http://ionicframework.com) and [Angular 1](https://angularjs.org/).

## Usage
The last version is proposed on this repository, and is configured to be functional with our own system. Please follow the instructions under the doc directory to set up your own environment.
