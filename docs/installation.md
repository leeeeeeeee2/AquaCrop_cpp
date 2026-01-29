# Installation Guide

This guide provides detailed instructions for building and installing AquaCrop C++ on various platforms.

## Prerequisites

### System Requirements

- **Operating System**: Linux, macOS, or Windows
- **Compiler**: C++17 compatible (GCC 9+, Clang 10+, MSVC 2019+)
- **Build System**: CMake 3.10 or newer
- **Disk Space**: ~100 MB for the project

### Platform-Specific Requirements

#### Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install build-essential cmake
```

#### macOS

```bash
# Using Homebrew
brew install cmake

# Or install Xcode command line tools
xcode-select --install
```

#### Windows

- Install [Visual Studio 2019](https://visualstudio.microsoft.com/) or newer
- Ensure CMake is added to PATH
- Or use [MSYS2](https://www.msys2.org/) with MinGW

## C++ Installation

### Basic Build

```bash
# Clone the repository
git clone https://github.com/your-org/AquaCrop_cpp.git
cd AquaCrop_cpp

# Create and enter build directory
mkdir build && cd build

# Configure the project
cmake ..

# Build the project
cmake --build .

# Run tests (optional)
ctest --output-on-failure
```

### Build Options

CMake supports several build options:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release       # Release mode (default: Debug)
cmake .. -DBUILD_SHARED_LIBS=ON           # Build shared libraries
cmake .. -DWITH_TESTS=ON                  # Enable tests (default: ON)
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to  # Custom install location
```

### Installation

```bash
# Install to the configured prefix
cmake --install .

# Or manually copy files
cp aquacrop_main /usr/local/bin/
```

## Python Wrapper Installation

### Prerequisites for Python

- Python 3.8 or newer
- pybind11 (included as a submodule or installed via pip)
- pip or conda for package management

### Using pip (Recommended)

```bash
# Clone with submodules
git clone --recursive https://github.com/your-org/AquaCrop_cpp.git
cd AquaCrop_cpp

# Create build directory
mkdir build && cd build

# Configure with Python support
cmake .. -DWITH_PYTHON=ON

# Build the extension
cmake --build .

# Install the Python package
cd ../python
pip install -e .
```

### Using conda

```bash
# Create conda environment
conda create -n aquacrop python=3.10 cmake pybind11
conda activate aquacrop

# Clone and build
git clone --recursive https://github.com/your-org/AquaCrop_cpp.git
cd AquaCrop_cpp

mkdir build && cd build
cmake .. -DWITH_PYTHON=ON -DPYTHON_EXECUTABLE=$(which python)
cmake --build .

# Install
cd ../python
pip install -e .
```

### Building from Source

For manual installation:

```bash
# From the python directory
cd python

# Install dependencies
pip install pybind11

# Build and install
python setup.py build
python setup.py install
```

Or using modern PEP 517 build:

```bash
pip install build
python -m build
pip install dist/aquacrop-*.whl
```

## Verification

### C++ Verification

```bash
# After building
./build/aquacrop_main

# Should output something like:
# AquaCrop C++ - Crop Water Productivity Model
# Projects loaded: None
```

### Python Verification

```python
import aquacrop

# Check version
print(aquacrop.__version__)

# Create a model instance
model = aquacrop.Model()

# Check if properly initialized
print("Model initialized successfully")
```

## Docker

A Dockerfile is provided for containerized deployment:

```dockerfile
# Build the image
docker build -t aquacrop .

# Run the container
docker run -it --rm -v $(pwd):/data aquacrop
```

## Troubleshooting

### CMake Errors

**Error: "Could not find CMake"**
- Ensure CMake is installed and in PATH
- Run `cmake --version` to verify

**Error: "C++ compiler not found"**
- Install a C++ compiler (gcc/clang/MSVC)
- On Linux: `sudo apt-get install build-essential`
- On macOS: Install Xcode command line tools

**Error: "C++17 not supported"**
- Update your compiler to a newer version
- GCC 9+, Clang 10+, or MSVC 2019+ support C++17

### Python Errors

**Error: "pybind11 not found"**
- Install pybind11: `pip install pybind11`
- Or use submodules: `git submodule update --init`

**Error: "Python version not supported"**
- Ensure Python 3.8+ is installed
- Check CMake Python detection: `cmake .. -DPYTHON_EXECUTABLE=$(which python)`

### Build Errors

**Error: "Permission denied"**
- Check write permissions in build directory
- Use `sudo` for system installation if needed

**Error: "Out of memory"**
- Reduce parallel build jobs: `cmake --build . -- -j 2`

## CI/CD

The project includes GitHub Actions workflows for automated testing:

- **Linux**: GCC and Clang builds
- **macOS**: Apple Clang builds
- **Windows**: MSVC builds

All builds include:
- Compilation tests
- Unit tests
- Python wrapper tests
- Code coverage reporting

## Performance Optimization

### Compiler Optimizations

```bash
# Release mode with optimizations
cmake .. -DCMAKE_BUILD_TYPE=Release

# Additional flags
cmake .. -DCMAKE_CXX_FLAGS="-O3 -march=native"
```

### Parallel Build

```bash
# Use all available cores
cmake --build . -- -j$(nproc)

# Or specify number of jobs
cmake --build . -- -j4
```

## Uninstalling

### C++ Uninstall

```bash
# If installed via cmake --install
cmake --install . --uninstall

# Or manually remove files
rm /usr/local/bin/aquacrop_main
```

### Python Uninstall

```bash
# Using pip
pip uninstall aquacrop

# Or remove from site-packages
rm -rf /path/to/python/site-packages/aquacrop
```
