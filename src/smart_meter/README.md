# Smart meter

This directory contains source files, documentation and build tools for the smart meter. 

## Purpose and general information

Smart meters provides drivers a way to pay for their parking without neccesarily having an internet connection on their own. Driver may interact with the smart meter with the driver app using BLE-connection. Smart meter then interacts with the cloud to provide the driver with price information on that specific area and a way to register for parking.

Being able to build smart meter requires certain preparations. Set up your environment following 
instructions [here.](https://github.com/DriverCity/SPARK/blob/master/src/smart_meter/doc/dev/development_environment_setup.md)

See the doc subdirectory for more detailed information about the smart meter.

## Dependencies

Smart meter depends on following 3rd party libraries. Some dependencies require preparations to be used. see [here](https://github.com/DriverCity/SPARK/blob/master/src/smart_meter/doc/dev/3rd_party_components.md) for more information.

C++ libraries:

- [GTest and GMock](https://github.com/google/googletest) (for running tests)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- [openssl](https://github.com/openssl/openssl)
- [curl](https://curl.haxx.se/)
- [sqlite3](http://sqlite.org/)

Node.js libraries:'

- [bleno](https://github.com/sandeepmistry/bleno)
- [deasync](https://www.npmjs.com/package/deasync)
- [noble](https://github.com/sandeepmistry/noble)
- [async](https://github.com/caolan/async)
- [util](https://nodejs.org/api/util.html)
