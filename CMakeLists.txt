cmake_minimum_required(VERSION 3.7)
project(DEMOranges)
set(CMAKE_CXX_STANDARD 11)

# Fix Windows path environment variable.
STRING(REGEX REPLACE "\\\\" "/" OCL_ROOT $ENV{OCL_ROOT})

# Set required OpenCL CMake build options.
set(OpenCL_INCLUDE_DIR ${OCL_ROOT}/include/)
set(OpenCL_LIBRARY ${OCL_ROOT}/lib/x86/OpenCL.lib)

# Add OpenCL stuff.
include_directories($ENV{OCL_ROOT}/include/)
link_libraries($ENV{OCL_ROOT}/lib/x86/OpenCL.lib)
find_package(OpenCL REQUIRED)

include_directories($ENV{OCL_ROOT}/include/CL/)

file(GLOB_RECURSE clEnqueueNDRangeKernel_SOURCE_FILES
        "util/*.h" "util/*.c"
        "structures/*.h"
        clEnqueueNDRangeKernel/host.c)
add_executable(clEnqueueNDRangeKernel ${clEnqueueNDRangeKernel_SOURCE_FILES})

file(GLOB_RECURSE test_compile_SOURCE_FILES
        "util/*.h" "util/*.c"
        "structures/*.h"
        kernels/test_compile.c)
add_executable(test_compile ${test_compile_SOURCE_FILES})