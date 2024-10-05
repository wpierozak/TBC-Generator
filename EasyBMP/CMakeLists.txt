# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(EasyBMP)

# Set the source directory
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INC_DIR "${CMAKE_SOURCE_DIR}/inc")

# Collect all .cpp files in the SRC_DIR directory
file(GLOB SRC_FILES "${SRC_DIR}/*.cpp")

# Add a library from the collected .cpp files
add_library(easybmp STATIC ${SRC_FILES})

# Optionally, specify include directories if needed
target_include_directories(easybmp PUBLIC "${INC_DIR}")

set_target_properties(easybmp PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)