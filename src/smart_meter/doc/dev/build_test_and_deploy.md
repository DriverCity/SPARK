# Build, test and deploy

## Introduction
The very first thing when starting implementing a new project, before writing a single line of actual code, 
is to make sure you can do following three things with your code: build, test and deploy. Being able to 
build the code is an obvious requirement, but the other two are just as important. If you don't have an easy 
way to test your code right from the start, tests are postponed and most likely not done at all. 
Having always an access to a distributable deployment of your software further helps testing and showing your 
progress to the customer any time. Deployment is especially important for cross-compiled code.

In SPARK's smart meter project, facilities for all three things exist right from the start, and are made very easy to use.
In fact, you can build, test and deploy with just a single command. This is done by running one of the three scripts 
in the root of the project.

## Using build scripts

### Basics
Smart meter's build system is based on CMake. Using CMake makes it possible to use different build variants and to compose 
the software from small components.
 
Smart meter has three build variants: linux-debug, raspberry pi -debug and raspberry pi -release. These variants have different 
compiler settings and slightly different set of components. Compiler options for each variant are specified in 
buildtools/variants directory. Component listings for each variant are specified in buildtools/components directory.

A component is the basic unit of compilation. It is a piece of code that can be tested independently, and compiled into a library 
or executable. Components are interconnected through their public interfaces. Splitting the program into components make it 
easy to test and manage. Using small components decreases compilation times significantly, makes it possible to test components 
independently and encourages developers to write clean, modular and easily testable code. In our use case, component division 
also enables us to build the project for linux desktop, even if that does not have required hardware compnents, such as 
Bluetooth radio. We simply replace the bluetooth component with a mock-up component in linux build.

### Running build scripts
There are three scripts in the root of the project. Each one for building, running tests for and deploying their specific 
build variant. Scripts and build variants are:

- build_linux_debug.sh : This script builds, tests and deploys the debug version for Linux desktop. You may use this variant
  for testing the code on your own desktop, not needing to cross-compile and run the code on the target hardware, which is convenient
  for development. This is the only variant supporting automated tests during the build.
  
- build_raspberrypi_debug.sh: This script builds and deploys the debug version of the software for Raspberry Pi 3.

- build_raspberrypi_release.sh: This script compiles and deploys the release version of the software for Raspberry Pi 3.

All scripts can be provided with extra arguments to modify their functionality. Formally, scripts can be called like:

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
test | Run tests for component. If any of the tests fail, the whole build fails. This option is ignored in raspberry variants.
all | Run tests and build component.

Following examples further clarify how to use build scripts.
```sh
# Build and deploy all components for raspberry debug version. Do not clean old builds (saves time)
./build_raspberrypi_debug.sh

# Delete all built and deployed components for raspberry release version.
./build_raspberrypi_release.sh clean-all

# Build and deploy component 'BLEModule' for raspberry debug version. Do not clean before build.
./build_raspberrypi_debug.sh -c BLEModule

# Clean component 'BLEModule' for raspberry debug version.
./build_raspberrypi_debug.sh -c BLEModule clean

# Re-build and deploy 'BLEModule' and all following components for raspberry debug version.
./build_raspberrypi_debug.sh --continue -c BLEModule clean build

# Run tests for component 'CloudConnection'
./build_linux_debug.sh -c CloudConnection test

# Clean, re-build, test and deploy all components.
./build_linux_debug.sh clean-all all
```

After running a build script, 'builds' and 'deploy' directories appear to the smart meter root directory. 
The builds directory contains object files, makefiles and all the other intermediate build results for each component. 
The deploy directory contains all libraries, executables and configuration files required to actually run the program.
In raspberry pi variants there is also a ready made zip-file you can upload, extract and run on Raspberry Pi.

## Making your own components

### Source tree

All components should be located in smart_meter/src directory. Each component must have its own sub-directory. In component's subdirectory, there will be up to three subdirectories:

- **include:** This subdirectory should contain all the public headers for the component. These headers will be automatically copied to the build directory, and are visible to other components. These headers form the component's public interface. If the component does not have any public interfaces, this sub-directory does not need to exist.

- **src**: This subdirectory should contain component's source files and those header files that are not visible to other components. This subdirectory also contains CMakeLists.txt for building the component. The CMakeLists.txt should be written according to [this template.](https://github.com/DriverCity/SPARK/blob/master/src/smart_meter/doc/dev/CMakeLists_src_template.txt) You may just copy the template and modify the necessary parts.

- **tests**: This subdirectory should contain source files for unit tests for the component. It also contains the CMakeLists.txt required to build the tests. The CMakeLists.txt file must be written according to [this template.](https://github.com/DriverCity/SPARK/blob/master/src/smart_meter/doc/dev/CMakeLists_tests_template.txt) If your test cases need some test files, put them to data-subdirectory under tests-directory. Using GTest as unit test framework is highly recommended. Try to implement your code in a way that it will be testable independent from other components. Also aim for high coverage when writing tests. Consider using [TDD](https://en.wikipedia.org/wiki/Test-driven_development), since it tends to help reaching these goals. If your component does not have any tests (!!) this subdirectory does not need to exist.

Summary: Your component source tree should look like this:
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

### Adding component to the build system
Just putting your source files to right place will not be enough to make buildtools notice it. There is one more thing left to do. You need to add your component to component lists of related build variants. This is not a hard task. Component listings for all build variants are located in buildtools/components directory. There are currently three component listings:

- **linux_components.py:** Add your component here, if it is specific to the Linux build (mock-ups).

- **raspberrypi_components.py:** Add your component here, if it is specific to Raspberry Pi (low level hw control).

- **common_components.py:** Add your component here, if your component is platform independent, you want to include it to all builds.

It would sound reasonable to put all platform-independent components to common components. There is one restriction however: Components need to be listed in order of dependency. Components will be built in order they appear on the list. Common components are built first and then the components specific to the used build variant. If your common component depends on a system dependent component (actual and mock-up implementation), you will need to list it in both linux_components and raspberrypi_components.

Adding a component to a list is simple, and identical in all component lists. You just need to add one line of Python to the chosen file(s):
```python
components.componentList.append(component.Component(name='your_component_name', 
                                                    runTestsCmd='./your_test_project_name'))
```
The name parameter should be the same as the project name in the CMakeLists.txt in your src-directory. It should also be the same as the name of your component's root directory. The runTestsCmd parameter tells buildtools the command to run your tests with. If you only have one executable test suite (like in the template), the command will match the project name (plus './' at the start). The name parameter is mandatory. Set runTestCmd parameter to None if your component has no tests (!!).

After this, you can now build and deploy your component and run your tests with build scripts:

```
./build_linux_debug.sh -c your_component_name all
```

### Referencing other components
It may be necessary to use services from other components to implement your component. Accessing other components is very easy. In CMakeLists templates, there are easy instructions on how to link your component to other components.

Public headers from all components are automatically published to builds/variant_name/include directory. This directory is included to your component in CMakeLists templates. All you need to do to include a header from another component in your source code is to write:

```c++
#include "other_component_name/desired_header.h"
```


## Deploying on target device

This section describes how to upload and run the code on target devices.

### Raspberry Pi 3

1. To upload software to Raspberry Pi, you need to connect it to your computer either through router or directly using Ethernet cable. With old computers,
you may need an Ethernet cross-over cable for direct connection.

2. Get Raspberry Pi's IP address

  - **With router:** Check Raspberry Pi's IP address from your router.
  
  - **Direct connection:** Try pinging address **169.254.186.133** . If there is no connection, you need to connect display and keyboard to the Pi and get
    the eth0 IP address with **ifconfig** command. Note that establishing connection after connecting cable takes few moments.

3. Upload software to Pi. After building software, there will be **upload_raspberrypi_debug.sh** or **upload_raspberrypi_release.sh** in the deploy directory.
   If you are using direct connection and **169.254.186.133** was correct IP address, you may run the script without parameters. Else give the correct IP
   address as the first parameter. The script will send zip-file containing the software and installation scripts to Pi in /home/pi directory.
   
4. Log on Pi using SSH client of your choise (e.g. PuTTY) using the correct IP address. Default login name is **pi** and default password is **raspberry**.

5. Install the software. Change to pi's home directory and run commands:

  ```sh
  chmod a+x install_debug.sh
  ./install_debug.sh
  # or _release.sh for release version.
  ```
  
6. Run the software

  ```sh
  ./start_debug.sh
  # or ./start_release.sh
  ```
  
  
### Intel edison

Build variant for Intel Edison is not guaranteed to be developed during the early stage of development. The upload process however is the same as with Pi,
except that you have to connect Edison through router.