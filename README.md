# AquaCrop C++

A high-performance C++ implementation of the FAO AquaCrop crop-water productivity model.

## What is AquaCrop?

AquaCrop is a crop-water productivity model developed by the Food and Agriculture Organization (FAO) of the United Nations. It simulates the yield response of herbaceous crops to water availability and is designed to be:

- **Physically based**: Uses soil-water balance and plant growth processes
- **Water-driven**: Focuses on the relationship between water use and crop productivity
- **User-friendly**: Balance between accuracy and simplicity for practical applications
- **Globally applicable**: Validated across diverse climatic and agricultural conditions

AquaCrop C++ provides a modern, performant implementation of the AquaCrop model, suitable for:
- Large-scale simulations and sensitivity analyses
- Integration into decision support systems
- Research applications requiring fast computation
- Embedded systems and cloud deployment

## Features

- **Complete Crop Simulation**: Models crop growth, development, and yield formation
- **Soil Water Balance**: Tracks soil moisture dynamics with multiple soil layers
- **Climate Integration**: Handles temperature, precipitation, evapotranspiration, and CO2 effects
- **Irrigation Management**: Supports various irrigation strategies and scheduling
- **Multiple Crop Types**: Wheat, maize, rice, cotton, soybeans, and more
- **Parameter Flexibility**: Extensive configuration options for soils, crops, and management
- **Result Analysis**: Comprehensive output files with daily and seasonal results

## Credits

This project is a C++ port of the original Fortran-based AquaCrop model. We express our sincere gratitude to the original authors and maintainers at [KUL-RSDA/AquaCrop](https://github.com/KUL-RSDA/AquaCrop) for their excellent work on the reference implementation.

```
If you use this software, please cite:
- Steduto, P., Hsiao, T.C., Raes, D., Fereres, E. (2009) AquaCrop—The FAO
  crop model to simulate yield response to water: I. Concepts and
  underlying principles. Agron. J. 101(3): 426–437.
- Raes, D., Steduto, P., Hsiao, T.C., Fereres, E. (2009) AquaCrop—The
  FAO crop model to simulate yield response to water: II. Main algorithms
  and software description. Agron. J. 101(3): 438–447.
```

## Directory Structure

```
AquaCrop_cpp/
├── src/                    # Source code
│   ├── main.cpp           # Entry point
│   ├── Global.cpp         # Global constants and utilities
│   ├── StartUnit.cpp      # Program initialization
│   ├── Run.cpp            # Simulation execution
│   ├── Simul.cpp          # Simulation modules
│   ├── ProjectInput.cpp   # Project file parsing
│   └── ...                # Other modules
├── include/               # Public headers
│   └── AquaCrop/          # Header files organized by module
├── python/                # Python wrapper
│   ├── _core.cpp          # pybind11 bindings
│   ├── aquacrop/          # Python package
│   ├── setup.py           # Installation script
│   └── pyproject.toml     # Modern package config
├── docs/                  # Documentation
│   ├── installation.md    # Build instructions
│   ├── usage.md           # Usage guide
│   ├── architecture.md    # Model architecture
│   ├── api.md             # API reference
│   └── README.md          # Documentation overview
├── PARAM/                 # Parameter files
├── LIST/                  # Project lists
├── test/                  # Test cases
├── CMakeLists.txt         # CMake build configuration
└── LICENSE                # MIT License
```

## Quick Start

### Prerequisites

- C++17 compatible compiler (GCC 9+, Clang 10+, MSVC 2019+)
- CMake 3.10 or newer
- For Python wrapper: Python 3.8+ and pybind11

### Building (C++)

```bash
# Clone the repository
git clone https://github.com/your-org/AquaCrop_cpp.git
cd AquaCrop_cpp

# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build
cmake --build .

# Run tests
ctest --output-on-failure
```

### Building with Python Wrapper

```bash
# From the repository root
mkdir build && cd build

# Configure with Python support
cmake .. -DWITH_PYTHON=ON

# Build
cmake --build .

# Install Python package
cd ../python
pip install -e .
```

### Running the Model

1. Create or modify a project file in `PARAM/`
2. Add the project filename to `LIST/ListProjects.txt`
3. Run the executable:

```bash
./build/aquacrop_main
```

## Documentation

Comprehensive documentation is available in the [docs/](docs/) directory:

- **[Installation Guide](docs/installation.md)**: Detailed build instructions
- **[Usage Guide](docs/usage.md)**: How to run simulations and interpret results
- **[Architecture](docs/architecture.md)**: Model design and data flow
- **[API Reference](docs/api.md)**: Complete API documentation

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## References

### FAO Publications

1. Steduto, P., Hsiao, T.C., Raes, D., Fereres, E. (2009) AquaCrop—The FAO crop model to simulate yield response to water: I. Concepts and underlying principles. Agronomy Journal 101(3): 426-437.

2. Raes, D., Steduto, P., Hsiao, T.C., Fereres, E. (2009) AquaCrop—The FAO crop model to simulate yield response to water: II. Main algorithms and software description. Agronomy Journal 101(3): 438-447.

3. Hsiao, T.C., Heng, L., Steduto, P., Rojas-Lara, B., Raes, D., Fereres, E. (2009) AquaCrop—The FAO crop model to simulate yield response to water: III. Parameterization and testing for maize. Agronomy Journal 101(3): 448-459.

4. Farahani, H.J., Izzi, G., Oweis, T.Y. (2009) Parameterization and evaluation of AquaCrop for full and deficit irrigated cotton. Agronomy Journal 101(3): 469-476.

### Official Resources

- **FAO AquaCrop Website**: https://www.fao.org/aquacrop/
- **Reference Manual**: AquaCrop Reference Manual (FAO)
- **Original Repository**: https://github.com/KUL-RSDA/AquaCrop

## Contributing

Contributions are welcome! Please see our [Contributing Guide](docs/guides/CONTRIBUTING.md) for details on:
- Coding standards
- Testing requirements
- Submission process
- Code of conduct

## Acknowledgments

- FAO for developing the original AquaCrop model
- KUL-RSDA for the reference Fortran implementation
- The agricultural modeling community for continuous improvements

---

**Note**: AquaCrop C++ is an independent implementation. While it aims to be consistent with the FAO reference implementation, results may vary slightly due to implementation differences and ongoing development.
