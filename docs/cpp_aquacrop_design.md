# C++ AquaCrop Design Document

This document outlines the design decisions and architecture for the C++ port of the AquaCrop model.

## Goals
- Provide a faithful yet modern C++ port of AquaCrop concepts.
- Ensure modularity, testability, and performance suitable for scientific workloads.
- Minimize regressions from the original Fortran design while leveraging C++ strengths (types, templates, RAII).

## Architecture Overview
- Core simulation loop operates on a time-stepped cycle updating soil, canopy, climate, and irrigation state.
- Components are exposed as modular headers under include/ with implementation separated under src/.
- Emphasis on clear interfaces and data ownership to enable easy unit testing and potential extensions.

## Modules and Interfaces
- Soil: moisture, temperature, salinity (if applicable), field capacity, water balance routines.
- Crop: phenology, biomass, canopy development, transpiration.
- Climate: weather inputs, radiation, temperature, rainfall; provides time-series data.
- Irrigation: water management policies and scheduling hooks.
- Core: shared utilities, units, and error handling abstractions.

## Data Models
- Lightweight structs for Weather, SoilState, CropState, and IrrigationState.
- Use strong typedefs or unit-safe wrappers where helpful to prevent unit errors.
- Public headers describe data layouts without leaking internal implementation details.

## Simulation Loop (High-level)
- Initialize state from input data.
- For each time step:
  - Update climate data for the step.
  - Compute soil moisture balance.
  - Update crop growth and phenology.
  - Apply irrigation policies and update water application.
  - Advance to next step and record outputs.

## API Design Considerations
- Public API exposed via include/aquacrop/*.hpp.
- Functions should be pure where possible and side-effect free for deterministic unit tests.
- Error handling via exceptions for unrecoverable errors; return codes for routine checks where exceptions are undesirable.
- ABI stability focus for minor releases; breakages should be intentional and versioned.

## Build and Portability
- Use CMake as the build system, with targets for library and samples/executables.
- Favor cross-platform standard libs; avoid platform-specific features when possible.
- Provide minimal dependency surface; document optional features and how to enable them.

## Testing Strategy
- Unit tests for core math, soil-water balance, and crop growth algorithms.
- Integration tests that exercise a small timeline with known expected outputs.
- Deterministic tests with fixed seeds for stochastic-like components.

## Extensibility
- Design with pluggable components for soil, crop, and climate as abstract interfaces.
- Allow easy substitution of models for sensitivity analyses.

## Example (Pseudo-API)
```
namespace aquacrop {
  struct Weather { double t, ra, tmin, tmax, precip; };
  struct SoilState { double theta; double theta_s; double theta_fc; };
  struct CropState { double biomass; double canopy_cover; int phenology; };

  class Model {
  public:
    void step(const Weather& w, SoilState& s, CropState& c);
  };
}
```

## Versioning and Documentation
- Document API changes in release notes; link to docs/design.md for architectural changes.
- Keep docs/cpp_aquacrop_design.md as a living document reflecting current design decisions.

## References
- This document complements docs/design.md and docs/api.md for a complete design narrative.
