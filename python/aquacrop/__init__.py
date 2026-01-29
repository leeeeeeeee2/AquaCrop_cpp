"""
AquaCrop - Python wrapper for the FAO AquaCrop crop-water productivity model.

This package provides a Python interface to the AquaCrop C++ model,
allowing easy configuration, execution, and analysis of crop simulations.

Example:
    >>> import aquacrop
    >>> model = aquacrop.Model()
    >>> model.set_soil("ClayLoam.sol")
    >>> model.set_climate("climate.csv")
    >>> model.set_crop("Wheat.Crop")
    >>> model.run()
    >>> results = model.get_results()
    >>> print(results.summary())
"""

__version__ = "1.0.0"
__author__ = "AquaCrop C++ Developers"

# Import main classes
from .model import Model
from .results import Results
from .parameters import (
    SoilParameters,
    ClimateParameters,
    CropParameters,
    IrrigationParameters,
    SimulationConfig,
)

# Import core module for direct access
from . import _core

# Define public API
__all__ = [
    # Version
    "__version__",
    
    # Main classes
    "Model",
    "Results",
    
    # Parameter classes
    "SoilParameters",
    "ClimateParameters",
    "CropParameters",
    "IrrigationParameters",
    "SimulationConfig",
    
    # Functions
    "version",
    "get_config",
    "set_log_level",
]

# Convenience functions
def version() -> str:
    """Get AquaCrop version.
    
    Returns:
        Version string.
    """
    return _core.version()

def get_config() -> dict:
    """Get current configuration.
    
    Returns:
        Configuration dictionary.
    """
    return _core.get_config()

def set_log_level(level: str) -> None:
    """Set logging level.
    
    Args:
        level: 'debug', 'info', 'warning', or 'error'.
    """
    valid_levels = ["debug", "info", "warning", "error"]
    if level not in valid_levels:
        raise ValueError(f"Invalid log level: {level}. Must be one of {valid_levels}")
    _core.set_log_level(level)

# Initialize logging
import logging

def _setup_logging():
    """Set up default logging configuration."""
    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s - %(name)s - %(levelname)s - %(message)s",
    )

_setup_logging()

# Clean up
del _setup_logging
del logging
