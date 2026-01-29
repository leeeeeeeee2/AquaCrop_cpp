# API Reference

This document provides complete API documentation for AquaCrop C++ and the Python wrapper.

## C++ API

### Namespace

All C++ code is in the `AquaCrop` namespace.

### Core Functions

#### Program Control

```cpp
namespace AquaCrop {

void StartTheProgram();
```
Initializes and runs the simulation program. Reads `ListProjects.txt` and processes all listed projects.

**Example:**
```cpp
#include "AquaCrop/StartUnit.h"

int main() {
    AquaCrop::StartTheProgram();
    return 0;
}
```

#### Initialization

```cpp
void InitializeTheProgram();
void FinalizeTheProgram();
```
Initialize and finalize the program, setting up paths and cleaning up resources.

#### Project Management

```cpp
int32_t GetNumberOfProjects();
std::string GetProjectFileName(int32_t iproject);
void GetProjectType(const std::string& TheProjectFile, 
                    typeproject& TheProjectType);
void InitializeProject(int32_t iproject, 
                       const std::string& TheProjectFile, 
                       typeproject TheProjectType);
```

#### Simulation Control

```cpp
void RunSimulation(const std::string& TheProjectFile, 
                   typeproject TheProjectType);
```

### Simulation Module

```cpp
namespace AquaCrop {

void Budget_module(int32_t DayNr, int32_t TargetTimeVal, int32_t TargetDepthVal,
    int32_t VirtualTimeCC, int32_t SumInterval, int32_t DayLastCut,
    int32_t NrDayGrow, int32_t Tadj, int32_t GDDTadj, dp GDDayi,
    dp CGCref, dp GDDCGCref, dp CO2i, dp CCxTotal, dp CCoTotal, dp CDCTotal,
    dp GDDCDCTotal, dp SumGDDadjCC, dp Coeffb0Salt, dp Coeffb1Salt, dp Coeffb2Salt,
    dp StressTotSaltPrev, dp DayFraction, dp GDDayFraction, dp FracAssim,
    int32_t StressSFadjNEW, bool StorageON, bool MobilizationON,
    dp& StressLeaf, dp& StressSenescence, dp& TimeSenescence,
    bool& NoMoreCrop, dp& TESTVAL);

void DeterminePotentialBiomass(int32_t VirtualTimeCC, dp SumGDDadjCC,
    dp CO2i, dp GDDayi, dp& CCxWitheredTpotNoS, dp& BiomassUnlim);

void DetermineBiomassAndYield(int32_t DayNr, dp ETo, dp Tmin, dp Tmax, dp CO2i,
    dp GDDayi, dp Tact, dp SumKcTop, dp CGCref, dp GDDCGCref,
    dp Coeffb0, dp Coeffb1, dp Coeffb2, dp FracBiomassPotSF,
    dp Coeffb0Salt, dp Coeffb1Salt, dp Coeffb2Salt, dp StressTot_Salt,
    dp SumGDDadjCC, dp CCiActual, dp FracAssim, int32_t VirtualTimeCC,
    int32_t SumInterval, dp& Biomass, dp& BiomassPot, dp& BiomassUnlim,
    dp& BiomassTot, dp& YieldPart, dp WPi, dp HItimesBEF,
    dp ScorAT1, dp ScorAT2, dp HItimesAT1, dp HItimesAT2, dp HItimesAT,
    dp alfaHI, dp alfaHIAdj, dp& SumKcTopStress, dp& SumKci,
    dp& WeedRCi, dp& CCiActualWeedInfested, dp& TactWeedInfested,
    int32_t& StressSFadjNEW, int32_t& PreviousStressLevel,
    bool& Store, bool& Mobilize, dp& ToMobilize, dp& Bmobilized,
    dp& Bin, dp& Bout, dp& TESTVALY);

}
```

### Constants and Types

```cpp
namespace AquaCrop {
    using dp = double;
    using intEnum = int;

    constexpr dp equiv = 0.64;
    constexpr int32_t max_SoilLayers = 5;
    constexpr int32_t max_No_compartments = 12;
    constexpr dp undef_double = -9.9;
    constexpr int32_t undef_int = -9;
    constexpr dp PI = 3.1415926535;
    constexpr dp CO2Ref = 369.41;
    constexpr dp EvapZmin = 15.0;
    constexpr dp eps = 10E-08;
    constexpr dp ac_zero_threshold = 0.000001;
}
```

### Enumerations

```cpp
enum class modeCycle : intEnum {
    GDDays = 0,
    CalendarDays = 1,
};

enum class pMethod : intEnum {
    NoCorrection = 0,
    FAOCorrection = 1,
};

enum class subkind : intEnum {
    Vegetative = 0,
    Grain = 1,
    Tuber = 2,
    Forage = 3,
};

enum class plant : intEnum {
    seed = 0,
    transplant = 1,
    regrowth = 2,
};

enum class Method : intEnum {
    full = 0,
    usda = 1,
    percentage = 2,
};

enum class EffectiveRainMethod : intEnum {
    full = 0,
    usda = 1,
    percentage = 2,
};

enum class TimeCuttings : intEnum {
    NA = 0,
    IntDay = 1,
    IntGDD = 2,
    DryB = 3,
    DryY = 4,
    FreshY = 5,
};

enum class Criterion : intEnum {
    CumulRain = 0,
    RainPeriod = 1,
    RainDecade = 2,
    RainVsETo = 3,
};

enum class AirTCriterion : intEnum {
    TminPeriod = 0,
    TmeanPeriod = 1,
    GDDPeriod = 2,
    CumulGDD = 3,
};

enum class GenerateTimeMode : intEnum {
    FixInt = 0,
    AllDepl = 1,
    AllRAW = 2,
    WaterBetweenBunds = 3,
};

enum class GenerateDepthMode : intEnum {
    ToFC = 0,
    FixDepth = 1,
};

enum class IrriMode : intEnum {
    NoIrri = 0,
    Manual = 1,
    Generate = 2,
};

enum class typeproject : intEnum {
    typenone = 0,
    typepro = 1,
    typeprm = 2,
};
```

## Python API

### Module

```python
import aquacrop
```

### Model Class

```python
class Model:
    """AquaCrop simulation model."""
    
    def __init__(self, config: Optional[dict] = None):
        """Initialize the model.
        
        Args:
            config: Optional configuration dictionary.
        """
    
    def set_soil(self, soil_file: str) -> None:
        """Set soil parameters from file.
        
        Args:
            soil_file: Path to soil parameter file (.sol).
        """
    
    def set_soil_parameters(self, params: dict) -> None:
        """Set soil parameters directly.
        
        Args:
            params: Dictionary of soil parameters.
                - field_capacity: Field capacity (fraction)
                - wilting_point: Wilting point (fraction)
                - saturation: Saturation point (fraction)
                - cn: Curve number
        """
    
    def set_climate(self, climate_file: str) -> None:
        """Set climate data from file.
        
        Args:
            climate_file: Path to climate file.
        """
    
    def set_climate_parameters(self, params: dict) -> None:
        """Set climate parameters directly.
        
        Args:
            params: Dictionary of climate file paths:
                - temperature_min: Tmin file
                - temperature_max: Tmax file
                - eto: ETo file
                - rainfall: Rainfall file
        """
    
    def set_crop(self, crop_file: str) -> None:
        """Set crop parameters from file.
        
        Args:
            crop_file: Path to crop parameter file (.Crop).
        """
    
    def set_crop_parameters(self, params: dict) -> None:
        """Set crop parameters directly.
        
        Args:
            params: Dictionary of crop parameters.
        """
    
    def set_irrigation(self, irrigation: dict) -> None:
        """Set irrigation parameters.
        
        Args:
            irrigation: Dictionary with:
                - method: 'manual', 'scheduled', or 'automatic'
                - amount: Irrigation amount (mm)
                - schedule: List of (day, amount) tuples
        """
    
    def set_start_date(self, date: str) -> None:
        """Set simulation start date.
        
        Args:
            date: Start date in YYYY-MM-DD format.
        """
    
    def set_end_date(self, date: str) -> None:
        """Set simulation end date.
        
        Args:
            date: End date in YYYY-MM-DD format.
        """
    
    def set_initial_conditions(self, conditions: dict) -> None:
        """Set initial soil water conditions.
        
        Args:
            conditions: Dictionary with:
                - swc: Initial soil water content (fraction)
                - depth: Initial root depth (m)
        """
    
    def run(self, project_file: Optional[str] = None) -> None:
        """Run the simulation.
        
        Args:
            project_file: Optional path to project file.
        """
    
    def get_results(self) -> Results:
        """Get simulation results.
        
        Returns:
            Results object containing all output data.
        """
```

### Results Class

```python
class Results:
    """Simulation results container."""
    
    def daily(self) -> pd.DataFrame:
        """Get daily results as DataFrame.
        
        Returns:
            DataFrame with daily simulation output.
        """
    
    def seasonal(self) -> dict:
        """Get seasonal summary.
        
        Returns:
            Dictionary with seasonal totals and averages.
        """
    
    def summary(self) -> str:
        """Get formatted summary string.
        
        Returns:
            Human-readable summary of results.
        """
    
    def export(self, filename: str, format: str = "csv") -> None:
        """Export results to file.
        
        Args:
            filename: Output file path.
            format: 'csv' or 'json'.
        """
    
    def export_csv(self, filename: str) -> None:
        """Export daily results to CSV.
        
        Args:
            filename: Output file path.
        """
    
    def export_json(self, filename: str) -> None:
        """Export results to JSON.
        
        Args:
            filename: Output file path.
        """
```

### Parameter Classes

```python
class SoilParameters:
    """Soil parameter container."""
    
    def __init__(self):
        self.field_capacity: float = 0.32
        self.wilting_point: float = 0.15
        self.saturation: float = 0.45
        self.cn: int = 70
        self.layers: List[SoilLayer] = []

class SoilLayer:
    """Individual soil layer."""
    
    def __init__(self, thickness: float, fc: float, wp: float, sat: float):
        self.thickness: float = thickness  # cm
        self.fc: float = fc  # Field capacity
        self.wp: float = wp  # Wilting point
        self.saturation: float = sat  # Saturation

class ClimateData:
    """Climate data container."""
    
    def __init__(self):
        self.tmin: pd.Series = None
        self.tmax: pd.Series = None
        self.eto: pd.Series = None
        self.rainfall: pd.Series = None

class CropParameters:
    """Crop parameter container."""
    
    def __init__(self):
        self.crop_name: str = ""
        self.planting_method: str = "seed"
        self.gdd_method: str = "calendar"
        self.harvest_index: float = 0.45
        self.canopy_growth_coeff: float = 0.012
        self.canopy_decline_coeff: float = 0.003
```

### Utility Functions

```python
def version() -> str:
    """Get AquaCrop version.
    
    Returns:
        Version string.
    """

def get_config() -> dict:
    """Get current configuration.
    
    Returns:
        Configuration dictionary.
    """

def set_log_level(level: str) -> None:
    """Set logging level.
    
    Args:
        level: 'debug', 'info', 'warning', or 'error'.
    """
```

## Usage Examples

### C++ Example

```cpp
#include "AquaCrop/StartUnit.h"
#include "AquaCrop/Global.h"
#include <iostream>

int main() {
    // Initialize
    AquaCrop::InitializeTheProgram();
    
    // Get projects
    int32_t n = AquaCrop::GetNumberOfProjects();
    std::cout << "Projects: " << n << std::endl;
    
    // Run each project
    for (int32_t i = 1; i <= n; i++) {
        std::string project = AquaCrop::GetProjectFileName(i);
        AquaCrop::typeproject type;
        AquaCrop::GetProjectType(project, type);
        AquaCrop::InitializeProject(i, project, type);
        AquaCrop::RunSimulation(project, type);
    }
    
    // Finalize
    AquaCrop::FinalizeTheProgram();
    
    return 0;
}
```

### Python Example

```python
import aquacrop

# Create model
model = aquacrop.Model()

# Configure
model.set_soil("ClayLoam.sol")
model.set_climate({
    "temperature_min": "tmin.csv",
    "temperature_max": "tmax.csv",
    "eto": "eto.csv",
    "rainfall": "rain.csv"
})
model.set_crop("Wheat.Crop")
model.set_start_date("1984-01-01")
model.set_end_date("1984-08-31")
model.set_irrigation({
    "method": "manual",
    "amount": 10
})

# Run
model.run()

# Get results
results = model.get_results()
print(results.summary())

# Export
results.export("results.csv")
```

### Parameter Study Example

```python
import aquacrop
import numpy as np

# Create base model
model = aquacrop.Model()
model.set_soil("ClayLoam.sol")
model.set_climate("climate.csv")
model.set_crop("Wheat.Crop")

# Vary irrigation
irrigation_levels = [0, 25, 50, 75, 100]
yields = []

for irri in irrigation_levels:
    model.set_irrigation({"method": "fixed", "amount": irri})
    model.run()
    results = model.get_results()
    yields.append(results.seasonal()['yield_kg_ha'])

# Print results
for irri, yield_ in zip(irrigation_levels, yields):
    print(f"Irrigation: {irri} mm, Yield: {yield_:.1f} kg/ha")
```

## Error Handling

### C++ Exceptions

```cpp
try {
    AquaCrop::InitializeTheProgram();
} catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
}
```

### Python Exceptions

```python
try:
    model.run()
except aquacrop.AquaCropError as e:
    print(f"Simulation error: {e}")
except FileNotFoundError as e:
    print(f"Input file not found: {e}")
except ValueError as e:
    print(f"Invalid parameter: {e}")
```

## Thread Safety

The C++ API is not thread-safe. Each thread should use its own Model instance.

The Python API is also not thread-safe. Use separate Model instances for parallel simulations.
