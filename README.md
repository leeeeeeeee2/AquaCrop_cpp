# AquaCrop-cpp

C++ port of the FAO AquaCrop crop-water productivity model.

This repository provides a lightweight C++ scaffolding for the AquaCrop model, along with guidance on how to contribute and how to document code provenance.

## Quick Start

- Prerequisites: a modern C++ toolchain and CMake (3.x or newer).
- Build:

```
mkdir build
cd build
cmake ..
cmake --build .
```
- Run (example):

```
./aquacrop_main
```

## Running and Building
- The project is structured to be portable across Linux/macOS. Windows users may need to adapt build steps to their toolchain.
- The build outputs depend on the CMake configuration (static vs shared libraries, executables, etc.).

## Directory Overview
 - src/: source code for core components (soil, crop, climate, irrigation, etc.).
 - include/: public headers.
 - tests/: unit tests.
 - docs/: documentation sources and generated content.
 - tools/: helper utilities and scripts (if any).
 - CMakeLists.txt: build configuration.
 - LICENSE: licensing information.
 
  classic/: Inplace placeholder folder for the classic Fortran AquaCrop model. This folder is not uploaded to the repository by design. If you need the classic Fortran sources, clone them manually into this folder in your local checkout and begin development there. The project will disable uploads to this folder.

## Documentation and Provenance
- This repository documents code provenance and AI-assisted edits to improve transparency.
- See the AI and Code Provenance section for details.
- Documentation about the repository structure and contribution guidelines lives in the docs/ directory.

### AI and Code Provenance
- Code Origin Policy: Distinguishes human-authored changes from AI-assisted edits. Annotate edits that originated from AI when feasible.
- AI Code Generator Discriminator: An experimental discriminator that classifies and logs edits produced by AI systems. Includes inputs, scoring, and interpretation guidance.
- Commit messages may include short provenance notes, e.g. [AI], to improve traceability.
- See docs/cpp_aquacrop_design.md for the dedicated C++ design document describing design decisions for AquaCrop port.
- OpenCode agent: See `agent.md` at repository root for an overview of how this agent assists with code and documentation tasks.

### Documentation of this repository
- Documentation sources live under docs/: API, design, guides, and tutorials.
- The structure is lightweight but extensible to support growth.

## Docs Structure (initial)
- See docs/README.md for an overview and contribution guidance.
- See docs/api.md for API surface reference.
- See docs/design.md for high-level design notes.
- See docs/guides/ for contributor guides and tutorials.
- See docs/cpp_aquacrop_design.md for the C++ design document describing AquaCrop port decisions.

## Contributing
- See CONTRIBUTING.md for guidance on contributing, coding standards, and testing requirements.
- Run tests via: `ctest --output-on-failure` or the project’s preferred test command.

## License
- See LICENSE for licensing information.

## Documentation of this repo
- Docs live under docs/ with an initial structure:
  - docs/README.md: overview of the documentation approach and how to contribute.
  - docs/api.md: API surface reference.
  - docs/design.md: high-level design decisions and module boundaries.
  - docs/guides/: usage guides and tutorials.
- A doc build may output HTML/PDF; see docs/BUILD.txt if present.

## Next steps
- Flesh out docs/api.md with the actual public headers (provide exact API surface once headers are finalized).
- Implement core module skeletons under include/ and src/ (soil, crop, climate, irrigation).
- Wire up CI for builds/tests and add a simple example app to validate the library.
- Expand docs/design.md with diagrams and more detailed interfaces.
- Use docs/progress_report.md as the living progress snapshot.
- Ensure AI provenance guidance is aligned with team practices.

## Acknowledgements 🙏
This C++ port and its documentation benefited from references and publicly available source materials from the KUL-RSDA AquaCrop project. Many thanks to the authors and maintainers of https://github.com/KUL-RSDA/AquaCrop for the source code and helpful reference material. (@explore)
