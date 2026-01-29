# AquaCrop C++ Documentation

Welcome to the AquaCrop C++ documentation. This comprehensive guide covers all aspects of the model from installation to advanced usage.

## Documentation Structure

| Document | Description |
|----------|-------------|
| [Installation](installation.md) | Detailed build instructions for C++ and Python |
| [Usage](usage.md) | How to run the model with input/output formats |
| [Architecture](architecture.md) | Model design, data flow, and algorithms |
| [API Reference](api.md) | Complete API documentation for C++ and Python |

## Quick Links

### Getting Started
- [Quick Start Guide](../README.md#quick-start)
- [Installation Instructions](installation.md)
- [Your First Simulation](usage.md#your-first-simulation)

### For Developers
- [Architecture Overview](architecture.md)
- [API Reference](api.md)
- [Contributing Guide](guides/CONTRIBUTING.md)

### For Python Users
- [Python Wrapper Installation](installation.md#python-wrapper)
- [Python API Usage](api.md#python-api)
- [Example Scripts](../python/)

## Overview

AquaCrop C++ is a C++ implementation of the FAO AquaCrop crop-water productivity model. It simulates the yield response of herbaceous crops to water availability using a physically-based approach that balances accuracy with computational efficiency.

### Key Features

- **High Performance**: Optimized C++ implementation for fast simulations
- **Cross-Platform**: Works on Linux, macOS, and Windows
- **Python Binding**: Full Python wrapper for easy integration
- **Modular Design**: Clean separation of concerns for easy extension
- **Well Documented**: Comprehensive documentation and examples

## Model Overview

AquaCrop simulates crop growth through several interconnected processes:

1. **Climate**: Processes weather data (rainfall, temperature, evapotranspiration)
2. **Soil Water Balance**: Tracks soil moisture across multiple soil layers
3. **Crop Development**: Simulates phenological development and canopy growth
4. **Biomass Accumulation**: Calculates biomass production from transpiration
5. **Yield Formation**: Models harvest index and yield determination

For detailed information, see the [Architecture](architecture.md) document.

## Directory Structure

```
docs/
├── README.md              # This file
├── installation.md        # Build instructions
├── usage.md              # Usage guide
├── architecture.md       # Model architecture
├── api.md                # API documentation
├── guides/
│   └── CONTRIBUTING.md   # Contribution guidelines
└── cpp_aquacrop_design.md  # C++ design document
```

## Getting Help

### Common Issues
- Check the [Installation Guide](installation.md) for build problems
- Review [Usage Examples](usage.md) for common workflows
- Look at test cases in `test/` directory

### Reporting Bugs
- Use GitHub Issues to report bugs
- Include your system information and error messages
- Provide minimal reproduction steps

### Contributing
- Read the [Contributing Guide](guides/CONTRIBUTING.md)
- Check open issues and PRs
- Follow coding standards and add tests

## Version Information

This documentation corresponds to AquaCrop C++ version 1.0. For the latest information, check the [GitHub repository](https://github.com/your-org/AquaCrop_cpp).

## Credits

This documentation and the C++ implementation are based on the excellent work of the [KUL-RSDA/AquaCrop](https://github.com/KUL-RSDA/AquaCrop) project. We thank them for their contributions to the agricultural modeling community.
