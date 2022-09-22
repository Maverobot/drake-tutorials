cmake_minimum_required(VERSION 3.5.1)

project(drake_tutorials)

find_program(LSB_RELEASE_EXEC lsb_release)
execute_process(COMMAND ${LSB_RELEASE_EXEC} -cs
  OUTPUT_VARIABLE LSB_RELEASE_CODENAME
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )
if(${LSB_RELEASE_CODENAME} STREQUAL "focal")
  message(STATUS "Using gcc-9 for Ubuntu 20.04")
  set(CMAKE_C_COMPILER /usr/bin/gcc-9)
  set(CMAKE_CXX_COMPILER /usr/bin/g++-9)
elseif(${LSB_RELEASE_CODENAME} STREQUAL "bionic")
  message(STATUS "Using gcc-7 for Ubuntu 18.04")
  set(CMAKE_C_COMPILER /usr/bin/gcc-7)
  set(CMAKE_CXX_COMPILER /usr/bin/g++-7)
else()
  message(WARNING "`Ubuntu ${LSB_RELEASE_CODENAME}` not supported")
endif()

find_package(drake CONFIG REQUIRED)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(./cmake/FetchMatplotlib.cmake)

add_subdirectory(mathematical_program)
add_subdirectory(modeling_dynamics_systems)
add_subdirectory(multibody_kinematics_and_dynamics)
