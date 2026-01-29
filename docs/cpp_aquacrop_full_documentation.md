# AquaCrop C++ — Full Documentation

## Overview ✅
This document describes the C++ implementation of the AquaCrop model included in this repository. It explains how to build and run the model, the high-level design and development workflow, and how to use the provided Python wrapper to run simulations and extract results programmatically.

---

## Repository layout 🔧
- `src/` — C++ source files implementing simulation modules (soil, crop, climate, simulation driver, utilities).
- `include/` — Public headers for the core modules.
- `PARAM/` — Example parameter projects and `ListProjects.txt` used by the console application.
- `build/` — CMake build directory (out-of-source build recommended).
- `docs/` — Design and usage documentation (this file lives here).
- `python/` — Python convenience wrapper and example scripts to call the compiled C++ executable.

---

## Building the C++ executable 🏗️
Prerequisites: a recent C++ toolchain and CMake (>= 3.10).

1. Create an out-of-source build directory:

```bash
mkdir -p build
cmake -S . -B build
cmake --build build --config Release
```

2. After a successful build the console executable created by this repo is named `aquacrop_main` and will be located inside the `build/` directory.

---

## Running the model from the command line ▶️
The executable reads project definitions from `PARAM/ListProjects.txt` (relative to your working directory). Each line should contain the path to a project, typically `case-*/project.ACp`.

Example:

```bash
# From the repository root
./build/aquacrop_main
```

Outputs and logs will be generated according to the repository’s output conventions (e.g., `OUTP/`, `SIMUL/`, per project `PARAM/case-*/output/`).

---

## C++ model design and internals 🧭
The C++ port follows a modular design by separating concerns across components:
- Climate processing (`ClimProcessing.*`)
- Temperature handling (`TempProcessing.*`)
- Initialization and project input (`InitialSettings.*`, `ProjectInput.*`)
- Simulation driver (`Simul.*`, `Run.*`)
- Output & information (`InfoResults.*`)

The simulation driver (`StartUnit::StartTheProgram`) builds projects from `PARAM/ListProjects.txt`, initializes parameters, and executes simulation runs in series. Core data structures are declared in header files under `include/AquaCrop/`.

---

## Development workflow and guidance 🛠️
- Keep changes small and well-tested. Use unit tests for deterministic components.
- Prefer out-of-source builds using `build/` to keep the repo root clean.
- When modifying or adding input formats, update the example files in `PARAM/` and the documentation.
- If adding a C++ API, provide clear header docs in `include/` and a short example in `docs/guides/`.

---

## Python wrapper — quick guide 🐍
The repository includes a lightweight Python wrapper that:
- Builds the C++ executable (if needed)
- Temporarily adjusts `PARAM/ListProjects.txt` to run a selected project and restores the file afterwards
- Runs the `aquacrop_main` executable and captures stdout/stderr

Files:
- `python/aquacrop_wrapper.py` — the wrapper API
- `python/run_example.py` — a simple usage example which calls the wrapper to run `case-01/project.ACp` and prints output.

Usage example from Python:

```python
from aquacrop_wrapper import AquaCropRunner
runner = AquaCropRunner(repo_root="/path/to/AquaCrop_cpp")
result = runner.run_project("case-01/project.ACp")
print(result.stdout)
```

Notes:
- The wrapper runs the executable from the repository root such that the program’s relative `PARAM/` path resolution works correctly.
- The wrapper is intentionally minimal: for heavier integration consider PyBind11/Cython if you need direct function calls into C++ (shared library/API), but that requires CMake changes and additional build dependencies.

---

## Example & verification ✅
- The `PARAM/` folder contains two example cases (`case-01/`, `case-02/`) with sample parameter files and output placeholders.
- Use `python/run_example.py` to demonstrate building and running a sample.

---

## Attribution & License 🙏
This project draws on concepts and references from several implementations of the AquaCrop model. Thanks to the authors and maintainers of the KUL-RSDA AquaCrop sources for their publicly available code and inspiration: https://github.com/KUL-RSDA/AquaCrop — thank you! (@explore)

All contributed files here follow the repository LICENSE (see `LICENSE`).

---

If you want, I can add a PyBind11-based wrapper that exposes internals as Python-callable functions (more integration work required). 💡
