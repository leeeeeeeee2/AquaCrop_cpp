# CMakeLists.txt for Python extension module
cmake_minimum_required(VERSION 3.10)

# Find Python
find_package(Python 3.8 REQUIRED COMPONENTS Interpreter Development)

# Find pybind11
find_package(pybind11 CONFIG REQUIRED)

# Create the Python extension module
pybind11_add_module(_core
    _core.cpp
)

# Set output directory
set_target_properties(_core PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/aquacrop
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/aquacrop
)

# Install targets
install(TARGETS _core
    DESTINATION aquacrop
)
