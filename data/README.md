# Data

This section groups all the open data and informations related used for our project

## Tampere city center pay car parks 

### Description

> **Origin :** Data available on the [following website](http://palvelut2.tampere.fi/tietovaranto/tietovaranto.php?id=133&alasivu=1) in finnish<br>
> **File :** TampereOpenDataEPSG4326.json

There data on Tampere's city center public pay car parks describes their position and the general parkings facilities characteristic.

### Relevant informations

- **ID** : parking area a unique number
- **NUMBER_OF_PLACES** :  Estimated number of seats for passenger cars
- **MAX_TIME** : longest permissible parking time in hours
- **PRICE** : EUR / hour (depending on the payment zone)
- **TIME_END_WEEKDAYS, TIME_END_SATURDAY, TIME_END_SUNDAY** : limitation period start in hours
- **TIME_START_WEEKDAYS, TIME_START_SATURDAY, TIME_START_SUNDAY** : limitation period expire in hours
- **DESTINATION_TYPE** : everyday restriction of the area (M = pay parking, D = disc parking and L = unlimited parking)
- **COORDINATES** : World coordinate system EPSG:4326 (Used by Google Map)