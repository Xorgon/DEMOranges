# DEMOranges
DEMOranges is a Discrete Element Method implementation written in C with OpenCL.
Building and compiling is handled by CMake.

This implementation was created for my third year Individual Project, the project report can be found [here](https://github.com/Xorgon/IP-Report).
The purpose of this implementation is to achieve a functional DEM implementation for large numbers of particles (10^7) that runs in approximately O(N) time.

The project structure and build instructions are below. Further documentation can be found in the Wiki [here](https://github.com/Xorgon/DEMOranges/wiki).

Some of the OpenCL utility functions in this project are constructed from code in Andrew Chow's project [here](https://github.com/chowder/IP-particle-fluid-sim).
## Project Structure
The project is organised into relevant directories. The contents of each of these is described below.

#### analysis/
This directory contains all of the analysis tools used to post-process data from simulations.
The primary functionality is the `agg_analysis.py` tools which are used to analyse, store, and graph agglomerate data.
This directory also contains some useful ParaView scripts that can be run to generate some useful geometry that might be frequently used.

#### kernels/
This directory contains all of the OpenCL kernels used in the project.
When running the project this directory must be in the same directory as the working directory so that the program can access the kernel files.
Within this directory are also some useful `get_gravity` and `get_vel_fluid` functions. The use of these is explained later.
Note, the kernel utilities file is in `util`, not `kernels`.

####  sims/
This directory contains all simulation programs as well as the main `simRunner` files.
`simRunner` is accessed by the simulations and handles all of the simulation running code.
A variety of simulations are available and are all CMake targets.

#### structures/
This directory contains all of the data structures used in the host code.
Matching device structures are found in `util/kernelUtils.cl`.

#### tests/
This directory contains all of the unit tests for the project.
These are accessed from `test/run_tests/` in simulations or can be run separately with more debug messages with the `run_tests` target.

#### util/
This directory contains all of the utility functions used in the project.
Most of these are found in `util/clUtils/` which contains all of the OpenCL utility functions that make calling OpenCL functions much simpler.
This directory also contains `kernelUtils.cl` which contains all of the utility functions and data structures for the OpenCL kernels.

#### verification/
This directory contains all of the verification test cases.
Actual simulations can be run using their targets and the resulting data can be graphed against the analytic solutions with the Python script in each directory.

## Building the project
This project uses CMake to easily build and compile. Building and compiling has been tested with the following systems:

- Windows, MSVC, AMD
- Windows, gcc, AMD
- Windows, MSVC, NVIDIA
- Windows, gcc, NVIDIA
- Linux, gcc, NVIDIA

For Linux systems CMake should be able to find the OpenCL SDK with `find_package(OpenCL REQUIRED)` however on Windows an Environment Variable `OCL_ROOT` may need to be set that points to the diretory containing the lib and include directories.
Windows compilation has only been tested with the AMD OpenCL SDK installed so the NVIDIA OpenCL SDK may need slight modifications to `CMakeLists.txt`.

There are targets for each verification test case and simulation as well as a unit test runner.
To add a new simulation simply add the following code to `CMakeLists.txt` and replace the name and main file (indicated by <>):
```
file(GLOB_RECURSE <target_name>_SOURCE_FILES <simulation .c file location>)
add_executable(<target_name> ${<target_name>_SOURCE_FILES} ${sim_SOURCE_FILES})
target_link_libraries(<target_name> ${OpenCL_LIBRARY})
if (NOT(WIN32))
    target_link_libraries(<target_name> m)
endif()
```
