# Design Overview

High-level architecture and module boundaries for AquaCrop-cpp.

## Modules
- soil: soil water balance and properties.
- crop: canopy growth, phenology, and biomass modeling.
- climate: weather inputs and seasonality.
- irrigation: water management strategies.
- core: shared utilities and interface abstractions.

## Architecture
- Plugin-friendly component model to allow extensions without touching core.
- Clear separation between public headers and internal implementation.
- Emphasis on testability and deterministic behavior for scientific computations.

## Interfaces
- Public interfaces expose lightweight data structures and functions with well-defined contracts.
- Error handling uses exceptions or error codes as per project guidelines.

## Testing and Quality
- Emphasize unit tests for core algorithms and integration tests for end-to-end scenarios.
- Deterministic builds across platforms is a goal.
