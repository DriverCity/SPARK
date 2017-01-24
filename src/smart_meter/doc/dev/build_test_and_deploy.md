# Build, test and deploy

## Introduction
The very first thing when starting implementing a new project, before writing a single line of actual code, 
is to make sure you can do following three things with your code: build, test and deploy. Being able to 
build the code is an obvious requirement, but the other two are just as important. If you don't have an easy 
way to test your code right from the start, tests are postponed and most likely not done at all. 
Having always an access to a distributable deployment of your software further helps testing and showing your 
progress to the customer any time. Deployment is especially important for cross-compiled code.

In SPARK's smart meter project, facilities for all three things exist right from the start, and are made very easy to use.
In fact, you can build, test and deploy with just a single command. This is done by running one of the build scripts 
in the root of the project.

## Using build scripts

### Basics
 
Smart meter has currently four build variants: linux-debug, raspberrypi-debug, raspberrypi-release and edison-debug. These variants have different compiler settings and slightly different set of components. Compiler options for each variant are specified in 
buildtools/variants directory. Component listings for each variant are specified in buildtools/components directory.

A component is the basic unit of compilation. It is a piece of code that can be tested independently, and compiled into a library 
or executable. Components are interconnected through their public interfaces. Splitting the program into components make it 
easy to test and manage. Using small components decreases compilation times significantly, makes it possible to test components 
independently and encourages developers to write clean, modular and easily testable code. In our use case, component division 
also enables us to build the project for multiple platforms, even if they didn't have required hardware compnents, such as 
Bluetooth radio. We simply replace the bluetooth component with a mock-up component in these builds.

### Running build scripts
The build scripts in the root of the project. Each builds and deploys their specific build variant. Scripts and build variants are:

- build_linux_debug.sh : This script builds, tests and deploys the debug version for Linux desktop. You may use this variant
  for testing the code on your own desktop, not needing to cross-compile and run the code on the target hardware, which is convenient
  for development.
  
- build_raspberrypi_debug.sh: This script builds and deploys the debug version of the software for Raspberry Pi 3.

- build_raspberrypi_release.sh: This script compiles and deploys the release version of the software for Raspberry Pi 3.

- build_linux_coverage: Build and run unit tests and create test coverage report. HTML-report will be generated in builds/linux_coverage/report/index.html. Extra parameters defined in following paragraphs do not have effect on this script.

All scripts (except for coverage tests) can be provided with extra arguments to modify their function. Formally, scripts can be called like:

```
build_<variant>.sh [clean-all | [[--continue | --continue-after] -c <component_name> [clean]][build | test | all]
```

Optional argumets are explained in table below.

Argument | Description
--- | --- 
clean-all | Delete all previously built and deployed files. 
-c | Specify a single component to be built / tested. Convenient option when you have made changes to one component only. The component name should become immediately after this option.
--continue | After building the component specified with -c option, continue building all the following components.
--continue-after | Skip the component specified with -c option and build all components following it.
clean | Clean component.
build | Build component.
test | Run tests for component. If any of the tests fail, the whole build fails. This option is ignored in cross-compilation.
all | Run tests and build component.

Following examples further clarify how to use build scripts.
```sh
# Build and deploy all components for raspberry debug version. Do not clean old builds (saves time)
./build_raspberrypi_debug.sh

# Delete all built and deployed components for raspberry release version.
./build_raspberrypi_release.sh clean-all

# Build and deploy component 'BLEService' for raspberry debug version. Do not clean before build.
./build_raspberrypi_debug.sh -c BLEService

# Clean component 'BLEService' for raspberry debug version.
./build_raspberrypi_debug.sh -c BLEModule clean

# Re-build and deploy 'BLEModule' and all following components for raspberry debug version.
./build_raspberrypi_debug.sh --continue -c BLEModule clean build

# Run tests for component 'CloudService'
./build_linux_debug.sh -c CloudService test

# Clean, re-build, test and deploy all components.
./build_linux_debug.sh clean-all all
```

After running a build script, 'builds' and 'deploy' directories appear to the smart meter root directory. 
The builds directory contains object files, makefiles and all the other intermediate build results for each component. 
The deploy directory contains all libraries, executables and configuration files required to actually run the program.
In cross-compiled variants there is also a ready made zip-file you can upload, extract and run on Raspberry Pi.

## Making your own components

### Source tree

All components should be located in smart_meter/src directory. Each component must be in its own sub-directory. In component's subdirectory, following structure is recommended to be used in each component's directory:

- **include:** This subdirectory contains all the public headers for the component. These headers form the component's public interface. If the component does not have any public interfaces, this sub-directory does not need to exist.

- **src**: This subdirectory contains component's source files and those header files that are not visible to other components. This subdirectory may also contain CMakeLists.txt for building the component. The CMakeLists.txt should build the component and/or deploy public header files and other deliverables. See other components for reference.

- **tests**: This subdirectory contains source files for unit tests for the component. It also contains the CMakeLists.txt required to build the tests. If your test cases need some test files, put them to data-subdirectory under tests-directory. Using GTest as unit test framework is highly recommended. Try to implement your code in a way that it will be testable independent from other components. Also aim for high coverage when writing tests. Consider using [TDD](https://en.wikipedia.org/wiki/Test-driven_development), since it tends to help reaching these goals. If your component does not have any tests (!!) this subdirectory does not need to exist.

Summary: Your component source tree might look like this:
```
 smart_meter
   |-src
      |-other_components
      |-your_component
         |-include
         |  |-public_heders.h
         |
         |-src
         |  |-CMakeLists.txt
         |  |-private_headers.h
         |  |-sources.cpp
         |
         |-tests
            |-CMakeLists.txt
            |-test_sources.cpp
            |-data
               |-testfiles.txt
```

### Adding component to the build
Just putting your source files to right place will not be enough to make buildtools notice it. There is one more thing left to do. You need to add your component to component lists of relevant build variants. Component listings for all build variants are located in buildtools/components directory.

It would sound reasonable to put all platform-independent components to common components. The order of listed components matters: Components need to be listed in order of dependency. Components will be built in order they appear on the list.

Define your component as below
```python
addComponent(component.Component(name, srcDir, testCmd))
```
The name parameter should be the same as the project name in the CMakeLists.txt in your src-directory. The srcDir defines path to your CMakeLists.txt. If you used the same directory structure as above, this may be set to None. The testCmd parameter tells buildtools the command to run your tests with. If you only have one executable test executable, the command will match the test project name (plus './' at the start). Set runTestCmd parameter to None if your component has no tests (!!). See existing component definitions for an example.

After this, you can now build and deploy your component and run your tests with build scripts:

```
./build_linux_debug.sh -c your_component_name all
```

### Referencing other components
It may be necessary to use services from other components to implement your component. Accessing other components is very easy. In CMakeLists templates, there are easy instructions on how to link your component to other components.

Public headers from all components should be deployed to builds/variant_name/include directory. Include this directory to your component in your CMakeLists (see existing components for an example). All you need to do to include a header from another component in your source code is to write:

```c++
#include "other_component_name/header.h"
```


## Deploying on target device

This section describes how to upload and run the code on target devices.

### Raspberry Pi 3

1. To upload software to Raspberry Pi, you need to connect it to your computer either through router or directly using Ethernet cable. With old computers, you may need an Ethernet cross-over cable for direct connection.

2. Get Raspberry Pi's IP address

  - **With router:** Check Raspberry Pi's IP address from your router.
  
  - **Direct connection:** Try pinging address **169.254.186.133** . If there is no connection, you need to connect display and keyboard to the Pi and get the eth0 IP address with **ifconfig** command or use 3rd party tools to find connected device's IP. Note that establishing connection after connecting cable may take few moments.

3. Upload software to Pi: After building software, there will be **upload.sh** in the deploy directory. Give the correct IP address as the first parameter to script. The script will send zip-file containing the software and installation scripts to Pi in /home/pi directory.
   
4. Log on Pi using SSH client of your choise (e.g. PuTTY) using the correct IP address. Default login name is **pi** and default password is **raspberry**.

5. Install the software: Change to pi's home directory and run commands:

  ```sh
  chmod a+x install_debug.sh
  ./install_debug.sh
  # or _release.sh for release version.
  ```
  
6. Run the software

  ```sh
  sudo ./raspberrypi_debug/start.sh
  ```
  
  
### Intel edison

The upload process however is the same as with Pi, except that you have to connect Edison through router.
