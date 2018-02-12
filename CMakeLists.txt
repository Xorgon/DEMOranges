cmake_minimum_required(VERSION 3.7)
project(DEMOranges)
set(CMAKE_CXX_STANDARD 11)

MESSAGE(Compiler: \ ${CMAKE_CXX_COMPILER_ID} \ ${CMAKE_CXX_COMPILER_VERSION})
MESSAGE(CMake\ Version: \ ${CMAKE_VERSION})

# Fix Windows path environment variable.
if (WIN32)
    STRING(REGEX REPLACE "\\\\" "/" OCL_ROOT $ENV{OCL_ROOT})
endif(WIN32)
if (NOT ${OCL_ROOT} STREQUAL "")
    # Set required OpenCL CMake build options.
    set(OpenCL_INCLUDE_DIR ${OCL_ROOT}/include/)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64 bits
        set(OpenCL_LIBRARY ${OCL_ROOT}/lib/x86_64/OpenCL.lib)
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        # 32 bits
        set(OpenCL_LIBRARY ${OCL_ROOT}/lib/x86/OpenCL.lib)
    endif()


    # Add OpenCL stuff.
    find_package(OpenCL REQUIRED)
    #add_library(OpenCL ${OpenCL_LIBRARY})
    include_directories(${OpenCL_INCLUDE_DIR})
    #SET_TARGET_PROPERTIES(OpenCL PROPERTIES LINKER_LANGUAGE C)
else()
    MESSAGE(OCL_ROOT\ not\ found.)
    find_package(OpenCL REQUIRED)
    set(OpenCL_LIBRARY OpenCL::OpenCL)
endif()

# Targets

file(GLOB_RECURSE run_tests_SOURCE_FILES
        "util/*.h" "util/*.c"
        "structures/*.h"
        tests/*)
add_executable(run_tests ${run_tests_SOURCE_FILES} tests/run_tests.c)
target_link_libraries(run_tests ${OpenCL_LIBRARY})
if (NOT(WIN32))
    target_link_libraries(run_tests m)
endif()

#file(GLOB_RECURSE clEnqueueNDRangeKernel_SOURCE_FILES
#        "util/*.h" "util/*.c"
#        "structures/*.h"
#        clEnqueueNDRangeKernel/host.c)
#add_executable(clEnqueueNDRangeKernel ${clEnqueueNDRangeKernel_SOURCE_FILES})
#target_link_libraries(clEnqueueNDRangeKernel ${OpenCL_LIBRARY} m)

file(GLOB_RECURSE test_sim_SOURCE_FILES
        "util/*.h" "util/*.c"
        "util/particleUtils.c" "util/particleUtils.h"
        "structures/*.h" "util/kernelUtils.cl"
        sims/test_host.c)
add_executable(test_sim ${test_sim_SOURCE_FILES})
target_link_libraries(test_sim ${OpenCL_LIBRARY})
if (NOT(WIN32))
    target_link_libraries(run_tests m)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -O3 -funroll-loops ") # Optimization stuff, TODO: Investigate.
#SET(CMAKE_VERBOSE_MAKEFILE ON)
