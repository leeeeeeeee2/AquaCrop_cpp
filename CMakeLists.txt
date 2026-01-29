cmake_minimum_required(VERSION 3.10)

project(AquaCrop-cpp VERSION 1.0.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(include)

# Source files
file(GLOB SOURCES "src/*.cpp")

# Main executable
add_executable(aquacrop_main ${SOURCES})

# Set output directories
set_target_properties(aquacrop_main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Find Python (optional)
option(WITH_PYTHON "Build Python wrapper" OFF)

if(WITH_PYTHON)
    # Find Python components
    find_package(Python 3.8 REQUIRED COMPONENTS Interpreter Development)
    
    # Find pybind11
    find_package(pybind11 CONFIG REQUIRED)
    
    # Create Python wrapper directory
    set(PYTHON_WRAPPER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/python")
    
    # Build Python extension using pybind11
    pybind11_add_module(_core
        "${PYTHON_WRAPPER_DIR}/_core.cpp"
    )
    
    # Set output directory for Python extension
    set_target_properties(_core PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PYTHON_WRAPPER_DIR}/aquacrop
        RUNTIME_OUTPUT_DIRECTORY ${PYTHON_WRAPPER_DIR}/aquacrop
    )
    
    # Add dependency between executables
    add_dependencies(_core aquacrop_main)
    
    # Install targets
    install(TARGETS aquacrop_main
        RUNTIME DESTINATION bin
    )
    
    install(TARGETS _core
        DESTINATION aquacrop
    )
    
    # Print Python status
    message(STATUS "Python wrapper: enabled")
    message(STATUS "Python version: ${Python_VERSION}")
    message(STATUS "Python include dir: ${Python_INCLUDE_DIRS}")
else()
    message(STATUS "Python wrapper: disabled (use -DWITH_PYTHON=ON to enable)")
endif()

# Testing
enable_testing()
add_subdirectory(test)

# Compiler options
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Build type specific options
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0)
endif()

# Installation
include(GNUInstallDirs)

install(TARGETS aquacrop_main
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Configuration file
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/AquaCropConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/AquaCropConfig.cmake"
    @ONLY
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/AquaCropConfig.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/AquaCrop
)

# Summary
message(STATUS "")
message(STATUS "AquaCrop C++ Configuration Summary")
message(STATUS "===================================")
message(STATUS "Version:        ${PROJECT_VERSION}")
message(STATUS "C++ Standard:   ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type:     ${CMAKE_BUILD_TYPE}")
message(STATUS "Python Wrapper: ${WITH_PYTHON}")
message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Binary Dir:     ${CMAKE_BINARY_DIR}")
message(STATUS "Source Dir:     ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "")
