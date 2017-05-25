# FakeBLETool

FakeBLETool is a development tool for emulating bluetooth communication with smart meter.
The tool can be used for testing or debugging smart meter without setting up an actual bluetooth connection
with other devices. The tool can be used in the development environment or in the target system 
(Raspberry Pi / Intel Edison) This page describes how to use the tool.

## Building FakeBLETool

FakeBLETool is built and deployed among the smart meter. Choose the target system and run the appropriate
build script to build both smart meter and FakeBLETool. If you only want to build FakeBLETool, you may specify
that with build script's -c parameter.

Example:
```sh
#Build both smart meter and FakeBLETool for the development environment
./build_linux_debug all

#Build only FakeBLETool
./build_linux_debug -c FakeBLETool
```

## Using FakeBLETool

Start smart meter and FakeBLETool:
```sh
# install and start smart meter
cd deploy/linux_debug
./install.sh
./start.sh

# Start FakeBLETool in other terminal tab / window
./bin/FakeBLETool
```

Now you are able to write commands to FakeBLETool window. The commands will be passed
to the smart meter as if they had come via bluetooth. There are two legal commands:

- **price**: This command will request for price information. Smart meter may respond with:
  
  1. On success:
    Smart meter responds with semicolon-separated list containing three fields:
	**price per hour in EUR (float);time limit in minutes (int);parking time resulution in minutes (int)**
	
  2. Error message not containing semicolons if something goes wrong.

  
- **Park**

  This command registers a parking event. The command consists of six semicolon-separated fields:
  
  Field number | description | Example
  ---|---|---
  1 | Mandatory value: 'Park'. The first field identifies the command. | 
  2 | Car's register number. | ABC123
  3 | Parking start time in format 'yyyy-MM-dd hh:mm'. | 2016-11-28 13:30
  4 | Parking duration in minutes. | 90
  5 | Payment verifier's name. The cloud platform needs this to check payment validity. | MyVerifier
  6 | Payment transaction unique id. The cloud platform needs this to check payment validity. |id123456
  
  Example: Register car **ABC123** to park **90** minutes starting from **2016-11-28 13:30**. Payment is verified by 
  **MyVerifier** and the transaction id is **id123456**:
  
  ```
  Park;ABC123;2016-11-28 13:30;90;MyVerifier;id123456
  ```
  
  Smart meter will respond with either **OK** if registration was successful, or with an error messge if something went wrong.
  
  
You may stop FakeBLETool with **Ctrl + Z**.