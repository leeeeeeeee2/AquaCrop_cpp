# Model Architecture

This document describes the internal architecture of AquaCrop C++, including data flow, module organization, and key algorithms.

## Overview

AquaCrop C++ follows a modular architecture that separates concerns and enables easy extension. The model is organized into several major components that interact through well-defined interfaces.

## Module Structure

```
src/
├── main.cpp              # Entry point
├── Global.cpp            # Global constants and utilities
├── StartUnit.cpp         # Program initialization
├── Run.cpp               # Simulation execution
├── Simul.cpp             # Simulation modules
├── ProjectInput.cpp      # Project file parsing
├── InitialSettings.cpp   # Initial configuration
├── ClimProcessing.cpp    # Climate data processing
├── TempProcessing.cpp    # Temperature processing
├── PrepareFertilitySalinity.cpp  # Soil fertility/salinity
├── RootUnit.cpp          # Root zone calculations
├── InfoResults.cpp       # Results generation
└── Utils.cpp             # Utility functions
```

## Data Flow

### Input Processing Pipeline

```
Project File (.ACp)
        |
        v
    ProjectInput
        |
        +---> Soil Parameters
        +---> Climate Data
        +---> Crop Parameters
        +---> Irrigation Settings
        +---> Initial Conditions
        |
        v
    InitialSettings
        |
        v
    Climate Processing
        |
        v
    [Simulation Loop]
```

### Simulation Loop

```
Daily Time Step:
1. Get climate data (Tmin, Tmax, ETo, Rainfall)
2. Calculate growing degree days (GDD)
3. Update soil water balance
4. Calculate crop water stress
5. Update canopy cover (CC)
6. Calculate biomass production
7. Update phenology
8. Check for harvest conditions
9. Store daily results
```

## Core Components

### Climate Module

The climate module processes weather data:

- **Temperature**: Daily minimum and maximum temperatures
- **Evapotranspiration**: Reference ET (ETo) from FAO Penman-Monteith
- **Rainfall**: Daily precipitation amounts
- **CO2 Concentration**: Atmospheric CO2 levels

Key calculations:
- Growing Degree Days (GDD): `GDD = (Tmax + Tmin)/2 - Tbase`
- Reference ET: FAO Penman-Monteith equation

### Soil Module

The soil module manages soil properties and water balance:

**Soil Layers**: Up to 5 soil layers with configurable thickness

**Soil Properties**:
- Texture (sand, silt, clay percentages)
- Bulk density
- Organic matter content
- Field capacity (FC)
- Wilting point (WP)
- Saturation point (SAT)
- Saturated hydraulic conductivity (Ksat)

**Water Balance**:
```
TAW = (FC - WP) * Z * 10  # Total Available Water (mm)
RAW = p * TAW             # Readily Available Water
Dr = previous_Dr + P + I - ET - Dp  # Depletion
```

Where:
- `p` = fraction of TAW that can be depleted without stress
- `Z` = root depth (m)
- `Dr` = root zone depletion
- `P` = precipitation
- `I` = irrigation
- `ET` = actual evapotranspiration
- `Dp` = deep percolation

### Crop Module

The crop module simulates plant growth and development:

**Phenology**:
- Germination
- Canopy growth
- Flowering
- Yield formation
- Maturation
- Senescence

**Canopy Development**:
```
CC = CC0 * e^(CGC * t)  # Exponential growth phase
CC = CCx - CDC * t      # Linear decline phase
```

Where:
- `CC0` = initial canopy cover
- `CGC` = canopy growth coefficient
- `CCx` = maximum canopy cover
- `CDC` = canopy decline coefficient

**Biomass Production**:
```
WP = 15 g biomass per m2 of ET  # Water productivity
Biomass = WP * Tr
```

**Yield Formation**:
```
Yield = Biomass * HI
HI = HI0 * (1 - StressEffects)
```

Where:
- `HI` = harvest index
- `HI0` = potential harvest index
- `StressEffects` = reduction due to water stress

### Irrigation Module

The irrigation module manages water application:

**Irrigation Methods**:
- **Manual**: Fixed amounts at specified times
- **Generated**: Automatic scheduling based on depletion
- **Automatic**: Real-time irrigation triggers

**Irrigation Efficiency**:
```
NetIrrigation = GrossIrrigation * Efficiency
```

### Simulation Module

The main simulation orchestrator:

```cpp
void RunSimulation() {
    for (DayNr = StartDay; DayNr <= EndDay; DayNr++) {
        // Get climate data
        GetDailyClimate(DayNr);
        
        // Calculate GDD
        CalculateGDD();
        
        // Update soil water balance
        UpdateSoilWaterBalance();
        
        // Calculate crop processes
        UpdateCropGrowth();
        
        // Check for stress
        CalculateWaterStress();
        
        // Update biomass
        UpdateBiomass();
        
        // Store results
        StoreDailyResults();
        
        // Check harvest
        if (ShouldHarvest()) break;
    }
}
```

## Key Algorithms

### Soil Water Balance

The soil water balance tracks water movement in the root zone:

```
Rainfall/Runoff:
  Runoff = CN_Method(P, AMC)

Infiltration:
  Infiltration = P - Runoff

Percolation:
  Dp = if (SW > FC) then (SW - FC) else 0

Capillary Rise:
  Cr = f(groundwater_depth, soil_properties)

ETa = Ks * Kc * ETo  # Actual ET
```

### Water Stress Coefficients

Water stress affects crop processes through stress coefficients:

```
Ks = (TAW - Dr) / ((1 - p) * TAW)  # for transpiration
Ksw = f(Dr/TAW)  # for canopy growth
Ksh = f(Dr/TAW)  # for harvest index
```

### Biomass Accumulation

Biomass is calculated from transpiration:

```
Tr = Ks * Kc * ETo
Biomass = WP * Tr * CO2_Factor
```

CO2 adjustment:
```
CO2_Factor = 1 + 0.5 * (CO2 - CO2Ref) / CO2Ref
```

## Data Structures

### Global Constants

```cpp
namespace AquaCrop {
    constexpr dp equiv = 0.64;
    constexpr int32_t max_SoilLayers = 5;
    constexpr int32_t max_No_compartments = 12;
    constexpr dp PI = 3.1415926535;
    constexpr dp CO2Ref = 369.41;
}
```

### Enumerations

```cpp
enum class modeCycle { GDDays, CalendarDays };
enum class pMethod { NoCorrection, FAOCorrection };
enum class subkind { Vegetative, Grain, Tuber, Forage };
enum class plant { seed, transplant, regrowth };
enum class Method { full, usda, percentage };
enum class IrriMode { NoIrri, Manual, Generate };
```

## Input/Output Formats

### Input Files

**Project Files (.ACp)**:
```ini
[Project]
Description = Example
StartDate = 1984/01/01
EndDate = 1984/08/31

[Soil]
File = Soil/ClayLoam.sol

[Climate]
File = Climate/Climate.csv

[Crop]
File = Crop/Wheat.Crop
```

**Parameter Files**:
- Soil files (.sol)
- Climate files (.csv)
- Crop files (.Crop)
- Irrigation files (.IRR)

### Output Files

**Daily Output**:
```csv
Date,Day,GDD,CC,TAW,Dr,ET,ETo,Kc,Biomass,Yield
1984/01/01,1,10.5,0.01,100,0,2.1,3.2,0.65,0.0,0.0
```

**Seasonal Summary**:
```
Total rainfall: 450.2 mm
Total irrigation: 120.0 mm
Total ET: 580.5 mm
Total biomass: 12500 kg/ha
Grain yield: 5625 kg/ha
```

## Extension Points

The architecture supports several extension points:

### Adding New Crops

1. Create new crop parameter file
2. Define crop coefficients
3. Register in crop module

### Adding New Soils

1. Create new soil parameter file
2. Define soil hydraulic properties
3. Register in soil module

### Custom Output

1. Extend results module
2. Add new output formats
3. Register output handlers

## Performance Considerations

### Optimization Strategies

1. **Time Step**: Daily time step balances accuracy and speed
2. **Memory**: Pre-allocate arrays for soil layers and time series
3. **Computations**: Minimize expensive operations in inner loops

### Scalability

- Single simulation: < 1 second
- 100 simulations: ~30 seconds
- Parameter studies: Parallelize independent runs

## References

- Steduto, P., et al. (2009) Agronomy Journal 101(3): 426-437
- Raes, D., et al. (2009) Agronomy Journal 101(3): 438-447
- FAO AquaCrop Reference Manual
