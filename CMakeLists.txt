cmake_minimum_required(VERSION 3.7)
project(DEMOranges)
set(CMAKE_CXX_STANDARD 11)

# Fix Windows path environment variable.
if (WIN32)
    STRING(REGEX REPLACE "\\\\" "/" OCL_ROOT $ENV{OCL_ROOT})
endif(WIN32)

# Set required OpenCL CMake build options.
set(OpenCL_INCLUDE_DIR ${OCL_ROOT}/include/)
set(OpenCL_LIBRARY ${OCL_ROOT}/lib/x86_64/OpenCL.lib)

# Add OpenCL stuff.
find_package(OpenCL REQUIRED)
#add_library(OpenCL ${OpenCL_LIBRARY})
include_directories(${OpenCL_INCLUDE_DIR})
#SET_TARGET_PROPERTIES(OpenCL PROPERTIES LINKER_LANGUAGE C)


# Targets

file(GLOB_RECURSE run_tests_SOURCE_FILES
        "util/*.h" "util/*.c"
        "structures/*.h"
        tests/*)
add_executable(run_tests ${run_tests_SOURCE_FILES} tests/run_tests.c)
target_link_libraries(run_tests ${OpenCL_LIBRARY})

file(GLOB_RECURSE clEnqueueNDRangeKernel_SOURCE_FILES
        "util/*.h" "util/*.c"
        "structures/*.h"
        clEnqueueNDRangeKernel/host.c)
add_executable(clEnqueueNDRangeKernel ${clEnqueueNDRangeKernel_SOURCE_FILES})
target_link_libraries(clEnqueueNDRangeKernel ${OpenCL_LIBRARY})

file(GLOB_RECURSE test_sim_SOURCE_FILES
        "util/*.h" "util/*.c"
        "util/particleUtils.c" "util/particleUtils.h"
        "structures/*.h" "util/kernelUtils.cl"
        sims/test_host.c)
add_executable(test_sim ${test_sim_SOURCE_FILES})
target_link_libraries(test_sim ${OpenCL_LIBRARY})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -O3 -funroll-loops ") # Optimization stuff, TODO: Investigate.

#SET(CMAKE_VERBOSE_MAKEFILE ON)
