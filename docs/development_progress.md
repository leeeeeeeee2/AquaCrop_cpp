# Development Progress

Overview
- Improvements to the project README and documentation scaffolding have been completed.
- Introduced an AI provenance policy and an AI code generator discriminator concept in docs.
- Added a dedicated C++ AquaCrop design document and a lightweight documentation suite under docs/.

Current status
- Repository layout established: include/, src/, tests/, docs/, tools/ with a minimal CMake-based build skeleton.
- Pro provenance and origin policies are documented in README (AI notes) and a dedicated design doc exists.
- Initial API/design docs prepared (docs/api.md, docs/design.md) and linked from README.
- Documentation for contributing to docs and guides added (docs/guides/CONTRIBUTING.md, docs/README.md).

Recent changes (high-level)
- Verified and fixed the C++ model execution path, allowing it to correctly locate project files in `PARAM/`.
- Fixed a segmentation fault in `calculate_saltcontent` by adding bounds checking for soil compartments.
- Implemented placeholder output for biomass, canopy, transpiration, and soil moisture in `src/Run.cpp`.
- Aligned C++ simulation parameters with classic model test cases (14-day simulation).
- Replaced and expanded README with clearer Quick Start and provenance sections.

Ongoing work
- Populate docs/api.md with a complete API surface derived from include/ headers.
- Implement unit tests for core components (soil, crop, climate).
- Add a CI workflow (GitHub Actions) to build and test on PRs.
- Expand the design document with diagrams and concrete interfaces.
- Align commit messages with provenance notes and ensure reviewers can understand AI-assisted edits.

Risks and considerations
- The AI provenance policy needs alignment with team practices and may require reviewer buy-in.
- ABI/stability decisions for the C++ port should be tracked across major versions.

Next steps
- 1) Flesh out docs/api.md with real API references.
- 2) Add unit and integration tests.
- 3) Configure CI to run builds and tests.
- 4) Add examples and tutorials in docs/guides.
- 5) Keep documentation in sync with code changes via ongoing reviews.
