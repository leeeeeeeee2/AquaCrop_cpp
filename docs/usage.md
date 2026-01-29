# Usage Guide

This guide explains how to run AquaCrop C++ simulations, prepare input files, and interpret results.

## Your First Simulation

### Step 1: Prepare Input Files

AquaCrop requires several input files:

1. **Project File** (`.ACp` or `.PRM`): Defines the simulation scenario
2. **ListProjects.txt**: Lists projects to run
3. **Parameter Files**: Soil, climate, crop, and management parameters

Example project file structure:

```
PARAM/
├── case-01/
│   └── wheat_example.ACp
├── ListProjects.txt
├── Soil/
├── Climate/
└── Crop/
```

### Step 2: Configure ListProjects.txt

Edit `LIST/ListProjects.txt` to specify which projects to run:

```
# Each line contains a project file path (relative to PARAM/)
case-01/wheat_example.ACp
```

### Step 3: Run the Model

**C++ Executable:**

```bash
./build/aquacrop_main
```

**Python:**

```python
import aquacrop

# Initialize model
model = aquacrop.Model()

# Run simulation
model.run("case-01/wheat_example.ACp")

# Access results
print(model.results.summary())
```

## Input File Formats

### Project Files (`.ACp`)

Project files define simulation parameters in an INI-style format:

```ini
[Project]
Description = Wheat simulation example
StartDate = 1984/01/01
EndDate = 1984/08/31

[Soil]
File = Soil/ClayLoam.sol

[Climate]
File = Climate/TminMaxET0.csv
RainfallFile = Climate/Rainfall.csv

[Crop]
File = Crop/Wheat.Crop

[InitialConditions]
SWC = 0.50  # Initial soil water content (fraction)

[Irrigation]
Method = Manual
Amount = 10  # mm per application
```

### Parameter Files

#### Soil Files (`.sol`)

```text
# Soil parameters
SoilName = ClayLoam
Clay = 30
Silt = 40
Sand = 30
OrganicMatter = 2.5
...

[Layer1]
Thickness = 30  # cm
FC = 0.32  # Field capacity
WP = 0.15  # Wilting point
Ksat = 20  # Saturated hydraulic conductivity

[Layer2]
Thickness = 70
FC = 0.35
WP = 0.18
Ksat = 15
```

#### Climate Files

**Temperature and ETo (CSV):**

```csv
Year,Month,Day,Tmin,Tmax,ETo
1984,1,1,5.2,15.3,3.2
1984,1,2,4.8,14.8,3.0
...
```

**Rainfall (CSV):**

```csv
Year,Month,Day,Rainfall
1984,1,1,0.0
1984,1,2,5.2
...
```

#### Crop Files (`.Crop`)

```text
[Crop]
CropName = Wheat
PlantingMethod = Seed
RowSpacing = 15
PlantDensity = 250

[Phenology]
GDDMethod = GDDays
CycleLength = 120  # days
...

[Growth]
CGC = 0.012  # Canopy growth coefficient
CDC = 0.003  # Canopy decline coefficient
HI = 0.45    # Harvest index
...

[Water]
KcFactor = 1.0
DroughtSensitivity = 0.5
...
```

## Output Files

### Daily Output Files

Daily simulation results are saved to `OUTP/Daily_*.csv`:

```csv
Date,Day,GDD,CC,TAW,Dr,ET,ETo,Kc,Biomass,Yield
1984/01/01,1,10.5,0.01,100,0,2.1,3.2,0.65,0.0,0.0
1984/01/02,2,11.2,0.02,100,0,2.2,3.4,0.65,0.1,0.0
...
```

### Seasonal Summary

Seasonal results are saved to `OUTP/Seasonal_*.txt`:

```
AquaCrop Version 1.0
Simulation: Wheat example
Period: 1984/01/01 - 1984/08/31

Seasonal Results:
------------------
Total rainfall:              450.2 mm
Total irrigation:            120.0 mm
Total ET:                    580.5 mm
Total biomass:               12500 kg/ha
Harvest index:               0.45
Grain yield:                 5625 kg/ha
Water productivity:          1.52 kg/m3
```

## Python API Usage

### Basic Workflow

```python
import aquacrop

# Initialize model
model = aquacrop.Model()

# Set parameters
model.set_soil("ClayLoam")
model.set_climate("climate.csv")
model.set_crop("Wheat")

# Configure simulation
model.set_start_date("1984-01-01")
model.set_end_date("1984-08-31")

# Run simulation
model.run()

# Get results
results = model.get_results()
print(results.seasonal_summary())
```

### Parameter Configuration

```python
# Soil parameters
model.set_soil_parameters({
    "field_capacity": 0.32,
    "wilting_point": 0.15,
    "saturation": 0.45,
    "cn": 70
})

# Climate parameters
model.set_climate({
    "temperature_min": "tmin.csv",
    "temperature_max": "tmax.csv",
    "eto": "eto.csv",
    "rainfall": "rain.csv"
})

# Crop parameters
model.set_crop_parameters({
    "planting_date": "1984-01-15",
    "gdd_method": "calendar",
    "harvest_index": 0.45,
    "canopy_growth_coeff": 0.012
})

# Irrigation
model.set_irrigation({
    "method": "scheduled",
    "schedule": [(30, 10), (60, 15), (90, 10)]
})
```

### Results Access

```python
# Daily results as DataFrame
daily_df = results.daily()
print(daily_df.head())

# Seasonal summary
summary = results.seasonal()
print(f"Yield: {summary['yield_kg_ha']} kg/ha")
print(f"Biomass: {summary['biomass_kg_ha']} kg/ha")
print(f"Water Use: {summary['et_mm']} mm")

# Export results
results.export("results.csv")
results.export_json("results.json")
```

## Advanced Usage

### Multiple Runs

```python
# Create parameter studies
param_study = aquacrop.ParameterStudy(model)

# Vary irrigation amounts
irrigation_levels = [0, 50, 100, 150, 200]

results = []
for irri in irrigation_levels:
    model.set_irrigation({"method": "fixed", "amount": irri})
    model.run()
    results.append({
        "irrigation": irri,
        "yield": model.get_results().seasonal()['yield_kg_ha']
    })

# Find optimal irrigation
optimal = max(results, key=lambda x: x['yield'])
print(f"Optimal irrigation: {optimal['irrigation']} mm")
```

### Sensitivity Analysis

```python
import numpy as np

# Define parameter ranges
params = {
    "field_capacity": np.linspace(0.25, 0.45, 10),
    "wilting_point": np.linspace(0.10, 0.25, 10)
}

# Run sensitivity analysis
for fc in params["field_capacity"]:
    model.set_soil_parameters({"field_capacity": fc})
    model.run()
    yield_ = model.get_results().seasonal()['yield_kg_ha']
    print(f"FC={fc:.2f}, Yield={yield_:.1f} kg/ha")
```

### Batch Processing

```python
import os
from pathlib import Path

# Process multiple projects
project_dir = Path("projects")
output_dir = Path("results")

for project_file in project_dir.glob("*.ACp"):
    model = aquacrop.Model()
    model.run(str(project_file))
    
    output_file = output_dir / f"{project_file.stem}_results.csv"
    model.get_results().export(output_file)
```

## Best Practices

1. **Input Validation**: Always validate input files before running
2. **Backup Projects**: Keep backups of important project configurations
3. **Version Control**: Track parameter file changes
4. **Documentation**: Document model setup and assumptions
5. **Testing**: Run test cases before production simulations

## Common Issues

### No Projects Found

Ensure `LIST/ListProjects.txt` exists and contains valid project paths# Check file exists
ls -la:

```bash
 LIST/ListProjects.txt

# Verify content
cat LIST/ListProjects.txt
```

### Missing Parameter Files

Verify all referenced parameter files exist:

```bash
# Check project references
grep -E "^File =" PARAM/*.ACp

# Verify files exist
for file in $(grep "^File =" PARAM/*.ACp | cut -d= -f2); do
    if [ ! -f "PARAM/$file" ]; then
        echo "Missing: PARAM/$file"
    fi
done
```

### Convergence Issues

For simulations with convergence problems:
- Reduce time step
- Check for unreasonable parameter values
- Review initial conditions

## Performance Tips

1. **Use Release Builds**: Always use `-DCMAKE_BUILD_TYPE=Release`
2. **Parallel Processing**: Use parameter studies for independent runs
3. **Cache Results**: Save intermediate results for debugging
4. **Profile Code**: Use `-DCMAKE_CXX_FLAGS="-pg"` for profiling
