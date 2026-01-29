# Future Development Guide

This document outlines the priority tasks for the ongoing development of the AquaCrop-cpp port.

### Task 1: Core Physics Implementation
Transition from placeholder logic to full implementation of the FAO AquaCrop physical models.
- Implement the comprehensive soil water balance (including infiltration, runoff, and redistribution).
- Port the crop growth and development algorithms (canopy expansion, senescence, and biomass production).
- Integrate the transpiration and soil evaporation engines.

### Task 2: Advanced Soil and Water Features
Enhance the model's capabilities to handle complex environmental conditions.
- Implement the full salt balance and salinity stress modules.
- Add groundwater table interaction and capillary rise logic.
- Support diverse irrigation management strategies (manual, generated, and net irrigation).

### Task 3: Comprehensive Data Handling and I/O
Improve the robustness and flexibility of project configuration and results reporting.
- Implement a robust parser for all AquaCrop file formats (.ACp, .SOL, .CRO, .CLI, etc.).
- Add support for structured output formats (e.g., CSV, JSON) for easier post-processing.
- Ensure strict validation of input data to prevent runtime errors.

### Task 4: Testing, Verification, and Performance
Establish a high-quality codebase with verified accuracy.
- Build a comprehensive suite of unit tests for individual physical equations.
- Perform automated regression testing against the classic Fortran AquaCrop model.
- Optimize performance-critical paths (e.g., soil compartment loops) to support large-scale spatial simulations.
