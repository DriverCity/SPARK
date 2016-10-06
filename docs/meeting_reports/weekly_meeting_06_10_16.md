# Project SPARK: Meeting Notes Thursday 6 Oct '16, 09:00 - 10:00

Attending: Soumya Das, Aurélien Wolz, Perttu Paarlahti, Bill Silverajan, Antti Kolehmainen
Absent: Hanning Zhau, Joona Luoma, Anil Özdemir, Tanja Lehtovaara

## Actions taken before meeting

* Google Cloud Platform service enabled for all project members (Trial version for 60 days).
* Raspberry Pi 3, SD cards, adapters, BLE dongles, plugs ordered.
* Github repo now hosts meeting notes

## Next steps

* Review Project Requirement documents Monday next week
* Share link to the User Stories on Google Docs

## Subjects discussed
### User stories
> Objectives: Concretize user stories to spot any misunderstandings / problems

While traveling through the user storie document, we have discuss about following topics :
- **Configuration**: Provide the possibility of use multiple beacons with only one application. Indeed, it's possible to have severals cars with beacons
- **BLE Pairing process**: We have to define the pairing process when we initiate a bluetooth interaction. It's possible to have pairing process with a bluetooth peripheral without pin code.
- **Open Data**: Antti provide a really interessting links for usable open data :
http://opentamperechallenge.fi/kilpailutyot/index.php#2
- **Security**: We have to define a security for the insurance of tokens' authenticity (using private keys on parking meters?)
- **Occupancy**: We can not have exhaustive data on the occupancy of parkings. The idea chosen is to implement a functionality for update occupancy based on the voluntary of drivers. After parking the vehicle, the driver can send information to the Cloud for notify the current occupation of his parking.

> **Keep in mind**

> Our general objective is to create something more simple than the traditional parking process if we want that someone use our system.

### Payment process

We discussed the payment process :
- Basically, for us payment like a black box : we just need to have a yes/no answer when user pays
- There are a lot of API available for payment, no re-implementation necessary
- It's possible mock-up for payment process
- Payment should be simple

### Project goals in customer's point of view
...

### Ending criteria
...

### Quiting criteria

We provide an application with lot of bugs and no-one want to use it
