# Smart Meter user manual

This document instructs how to install, configure and run smart meter application.

## Installation

Smart meter installation package is automatically created in build process for each supported target hardware.
This section focuses on Raspberry Pi (Raspbian Jessie) variant, as it's currently our only officially supported platform.

Follow instructions [here]() to compile smart meter for Raspberry Pi.
After compiling, all required files are located in the deploy directory. Upload installation package to Raspberry Pi
using upload.sh:

```sh
./deploy/raspberrypi_release/upload.sh IP-ADDRESS
```

Log on Raspberry Pi via SSH. Uploaded files are located in pi's home directory. Install smart meter running 
install_release.sh. The script expects one argument: Type 'clean', to run clean installation with dependencies, or 
'upgrade' to install smart meter application only. Use 'clean' for first time installation.

```sh
~/install_release.sh clean
```

Installing and using smart meter requires Internet access, so make sure you have set up the connection before installation.
Clean installation may take few minutes. While installing external dependencies, script may print vague error and warning messages. These can be ignored.
On success, script will print 'Installation completed successfully!' at the end.

## Configuration

After installation, you need to configure smart meter application. Configuration will give smart meter it's 
unique parking area id in addition to some other variables. To set configuration parameters, edit file 
'~/raspberrypi_release/config/global.txt'. Configuration parameters are given as semicolon-separated key-value pairs.
Required parameters with their descriptions are given in table below.

Key | Description | Notes | Example
---|---|---|---
parkingAreaId | Smart meter's unique id | An integer value | parkingAreaId;1022
BLEInputFifo | Communication FIFO from BLENode to smart meter application. | In current version, this has to be set to 'BLEInputFifo' | BLEInputFifo;BLEInputFifo
BLEResponseFifo | Communication FIFO from smart meter application to BLENode. | In current version, this has to be set to 'BLEResponseFifo' | BLEResponseFifo;BLEResponseFifo
eventDb | Name of SQLite database for temporarily storing parking events. | Not currently used. | eventDb;events.db
priceAPI | Http address for quering parking prices. | This address is supposed to be usable just appending area id to the address. | priceAPI;https://spark2-150308.firebaseio.com/parkingArea.json?orderBy=%22area_number%22&equalTo=
parkingEventAPI | Http address of REST API for registering a parking event. | | parkingEventAPI;https://spark2-150308.appspot-preview.com/api/v1.0/parkingEvent/
logFile | Name of application output log file | Log file does not need to exist. Directory does. Default directory is the bin-directory. | logFile;log.log
demoMode | Switch demo mode on (1) or off (0). This parameter is optional. Demo-mode is off by default. | Demo mode allows creating rare situations that would be hard to create otherwise. Intended for development purposes only. | demoMode;1

The order of parameters does not matter. Each key-value pair has to be on its own line. There may be empty lines and comments starting with '#'.
As a whole, global.txt may look like this:

```
# Example config-file.

logFile;log.log
BLEInputFifo;BLEInputFifo
BLEResponseFifo;BLEResponseFifo
eventDb;events.db
priceAPI;https://spark2-150308.firebaseio.com/parkingArea.json?orderBy=%22area_number%22&equalTo=
parkingEventAPI;https://spark2-150308.appspot-preview.com/api/v1.0/parkingEvent/
parkingAreaId;1022
demoMode;0
```

## Running the application

To run smart meter application, run ~/raspberrypi_release/start.sh as root. Root rights are needed to stop Raspberry Pi's bluetooth daemon, switch 
bluetooth adapter on and permit ping (not permitted in Raspbian by default). Smart meter starts working immediately. 
You may safely quit the application any time pressing ctrl+C. Quiting may take few seconds.
