cmake_minimum_required(VERSION 3.5)

# Project name and version
project(TBC_EB-PVD_Generator)

# Include GNUInstallDirs to use standard directory variables
include(GNUInstallDirs)
find_package(OpenMP REQUIRED)

# Set the output directories for binaries and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INC_DIR "${CMAKE_SOURCE_DIR}/inc")

add_subdirectory(EasyBMP)

file(GLOB SRC_FILES "${SRC_DIR}/*.cpp")

add_executable(tbc_generator ${SRC_FILES})

target_include_directories(tbc_generator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(tbc_generator easybmp OpenMP::OpenMP_CXX)
