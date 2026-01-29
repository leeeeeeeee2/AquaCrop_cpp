# AquaCrop C++ Parameter File Format Conversion

## Overview
The test case parameter files have been converted to be compatible with the AquaCrop C++ version input format.

## File Structure Changes

### Original Format (Simple Parameters)
```
param1=default_case01
param2=default
```

### C++ Compatible Format (.ACp files)
Based on the `ProjectInput.cpp` parsing logic, the C++ version expects a structured format with:
- Description line
- Version number
- Simulation dates (start/end day numbers)
- Crop dates
- 15 input sections with: info line, filename, directory

## Created Files

### 1. case-01/project.ACp
- Description: "Case 1 - AquaCrop C++ Test Project"
- Version: 7.2
- Simulation: Full year (days 1-365)
- Crop: Full year (days 1-365)

### 2. case-02/project.ACp
- Description: "Case 2 - AquaCrop C++ Test Project"
- Version: 7.2
- Simulation: Full year (days 1-365)
- Crop: Full year (days 1-365)

### 3. ListProjects.txt
Lists both project files to be processed by the C++ version:
```
case-01.ACp
case-02.ACp
```

## Input Sections Defined
Each project file includes 15 standard input sections:

1. **Climate** - `climate.TXT` in `CLIM/`
2. **Temperature** - `temperature.TXT` in `CLIM/`
3. **ETo** - `eto.TXT` in `CLIM/`
4. **Rain** - `rain.TXT` in `CLIM/`
5. **CO2** - `co2.TXT` in `CLIM/`
6. **Calendar** - `calendar.TXT` in `CLIM/`
7. **Crop** - `crop.TXT` in `CROP/`
8. **Irrigation** - `irrigation.TXT` in `IRRIG/`
9. **Management** - `management.TXT` in `MANAGE/`
10. **Soil** - `soil.TXT` in `SOIL/`
11. **Groundwater** - `groundwater.TXT` in `GWT/`
12. **SWC Initial** - `swcini.TXT` in `SOIL/`
13. **Off-season** - `offseason.TXT` in `OFF/`
14. **Observations** - `observations.TXT` in `OBS/`

## Directory Structure
```
/workspaces/AquaCrop-cpp/test/
├── ListProjects.txt
├── case-01/
│   ├── params.txt (original)
│   ├── output/ (results)
│   └── project.ACp (new C++ format)
└── case-02/
    ├── params.txt (original)
    ├── output/ (results)
    └── project.ACp (new C++ format)
```

## Usage
The C++ version will:
1. Read `ListProjects.txt` to find available projects
2. Parse each `.ACp` file for configuration
3. Load the specified input files from their directories
4. Run the simulation
5. Generate output in the `OUTP/` directory

## Next Steps
To complete the C++ setup:
1. Create the actual input data files referenced in the project files
2. Set up the required directory structure (CLIM/, CROP/, etc.)
3. Test with the C++ executable

---
*Converted on: January 28, 2026*
*Target: AquaCrop C++ version*