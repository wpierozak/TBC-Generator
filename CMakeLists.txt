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

file(GLOB SRC_FILES "${SRC_DIR}/*.cpp")

add_executable(tbc_generator ${SRC_FILES})

target_include_directories(tbc_generator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(tbc_generator OpenMP::OpenMP_CXX)

# Set the build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set optimization flags for Release build
if(CMAKE_BUILD_TYPE MATCHES Release)
    # For GCC or Clang
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s")
    # For MSVC
    elseif(MSVC)
        set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
    endif()
endif()

# Enable warnings (optional but recommended)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

# Installation configuration
install(TARGETS tbc_generator
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}  # This resolves to /usr/local/bin by default
)
