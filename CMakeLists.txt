cmake_minimum_required(VERSION 3.7)
project(DEMOranges)
set(CMAKE_CXX_STANDARD 11)

MESSAGE(Compiler: \ ${CMAKE_CXX_COMPILER_ID} \ ${CMAKE_CXX_COMPILER_VERSION})
MESSAGE(CMake\ Version: \ ${CMAKE_VERSION})

# Fix Windows path environment variable.
if (WIN32)
    if(DEFINED ENV{OCL_ROOT})
        STRING(REGEX REPLACE "\\\\" "/" OCL_ROOT $ENV{OCL_ROOT})
    endif()
    if(DEFINED ENV{CUDA_PATH})
        STRING(REGEX REPLACE "\\\\" "/" CUDA_PATH $ENV{CUDA_PATH})
    endif()
endif(WIN32)
if(DEFINED OCL_ROOT AND DEFINED CUDA_PATH)
    MESSAGE("Both AMD and NVIDIA OpenCL SDKs have been found. Defaulting to AMD.")
endif()
if (DEFINED OCL_ROOT)
    MESSAGE("Building with AMD OpenCL SDK.")
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
elseif(DEFINED CUDA_PATH)
    MESSAGE("Building with NVIDIA OpenCL SDK (CUDA Toolkit).")
    set(OpenCL_INCLUDE_DIR ${CUDA_PATH}/include/)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64 bits
        set(OpenCL_LIBRARY ${CUDA_PATH}/lib/x64/OpenCL.lib)
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        # 32 bits
        set(OpenCL_LIBRARY ${CUDA_PATH}/lib/Win32/OpenCL.lib)
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

# Allow PROJECT_DIR to be used in C source files (useful for runtime kernel compilation)
add_compile_definitions(PROJECT_DIR="${PROJECT_SOURCE_DIR}/")

# General source files

file(GLOB_RECURSE sim_SOURCE_FILES
        "util/*.h" "util/*.c"
        "util/particleUtils/particleUtils.c" "util/particleUtils/particleUtils.h"
        "structures/*.h" "util/kernelUtils.cl"
        "tests/*/*"
        "sims/simRunner/*")

# Targets

file(GLOB_RECURSE run_tests_SOURCE_FILES
        "util/*.h" "util/*.c"
        "structures/*.h"
        tests/*)
add_executable(run_tests ${run_tests_SOURCE_FILES} tests/standalone_tests.c)
target_link_libraries(run_tests ${OpenCL_LIBRARY})
if (NOT(WIN32))
    target_link_libraries(run_tests m)
endif()

file(GLOB_RECURSE particle_ball_SOURCE_FILES sims/particle_ball.c)
add_executable(particle_ball ${particle_ball_SOURCE_FILES} ${sim_SOURCE_FILES})
target_link_libraries(particle_ball ${OpenCL_LIBRARY})
if (NOT(WIN32))
    target_link_libraries(particle_ball m)
endif()

file(GLOB_RECURSE box_SOURCE_FILES sims/box.c)
add_executable(box ${box_SOURCE_FILES} ${sim_SOURCE_FILES})
target_link_libraries(box ${OpenCL_LIBRARY})
if (NOT(WIN32))
    target_link_libraries(box m)
endif()

file(GLOB_RECURSE tgv_box_SOURCE_FILES sims/tgv_box.c)
add_executable(tgv_box ${tgv_box_SOURCE_FILES} ${sim_SOURCE_FILES})
target_link_libraries(tgv_box ${OpenCL_LIBRARY})
if (NOT(WIN32))
    target_link_libraries(tgv_box m)
endif()

file(GLOB_RECURSE tgv_periodic_SOURCE_FILES sims/tgv_periodic.c)
add_executable(tgv_periodic ${tgv_periodic_SOURCE_FILES} ${sim_SOURCE_FILES})
target_link_libraries(tgv_periodic ${OpenCL_LIBRARY})
if (NOT(WIN32))
    target_link_libraries(tgv_periodic m)
endif()

file(GLOB_RECURSE multi_tgv_periodic_SOURCE_FILES sims/multi_tgv_periodic.c)
add_executable(multi_tgv_periodic ${multi_tgv_periodic_SOURCE_FILES} ${sim_SOURCE_FILES})
target_link_libraries(multi_tgv_periodic ${OpenCL_LIBRARY})
if (NOT(WIN32))
    target_link_libraries(multi_tgv_periodic m)
endif()

file(GLOB_RECURSE hourglass_SOURCE_FILES sims/hourglass.c)
add_executable(hourglass ${hourglass_SOURCE_FILES} ${sim_SOURCE_FILES})
target_link_libraries(hourglass ${OpenCL_LIBRARY})
if (NOT(WIN32))
    target_link_libraries(hourglass m)
endif()

file(GLOB_RECURSE normal_force_SOURCE_FILES verification/normal_force/normal_force_sim.c)
add_executable(normal_force ${normal_force_SOURCE_FILES} ${sim_SOURCE_FILES})
target_link_libraries(normal_force ${OpenCL_LIBRARY})
if (NOT(WIN32))
    target_link_libraries(normal_force m)
endif()

file(GLOB_RECURSE cohesion_SOURCE_FILES verification/cohesion/cohesion_sim.c)
add_executable(cohesion ${cohesion_SOURCE_FILES} ${sim_SOURCE_FILES})
target_link_libraries(cohesion ${OpenCL_LIBRARY})
if (NOT(WIN32))
    target_link_libraries(cohesion m)
endif()

file(GLOB_RECURSE friction_SOURCE_FILES verification/friction/friction_sim.c)
add_executable(friction ${friction_SOURCE_FILES} ${sim_SOURCE_FILES})
target_link_libraries(friction ${OpenCL_LIBRARY})
if (NOT(WIN32))
    target_link_libraries(friction m)
endif()

file(GLOB_RECURSE drag_SOURCE_FILES verification/drag/drag_sim.c)
add_executable(drag ${drag_SOURCE_FILES} ${sim_SOURCE_FILES})
target_link_libraries(drag ${OpenCL_LIBRARY})
if (NOT(WIN32))
    target_link_libraries(drag m)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -O3 -funroll-loops ") # Optimization stuff, TODO: Investigate.
#SET(CMAKE_VERBOSE_MAKEFILE ON)
