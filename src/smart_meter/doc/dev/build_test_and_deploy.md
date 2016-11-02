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
-c | Specify a single component to be built / tested. Convenient option when you have made changes to one component only. 
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