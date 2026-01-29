"""
Parameter classes for AquaCrop Python wrapper.

This module provides classes for configuring soil, climate, crop,
irrigation, and simulation parameters.
"""

from typing import Dict, List, Optional, Union
from dataclasses import dataclass, field
from datetime import datetime
import os
import csv


@dataclass
class SoilLayer:
    """Individual soil layer.
    
    Attributes:
        thickness: Layer thickness (cm).
        fc: Field capacity (fraction).
        wp: Wilting point (fraction).
        saturation: Saturation point (fraction).
    """
    
    thickness: float = 30.0
    fc: float = 0.32
    wp: float = 0.15
    saturation: float = 0.45


@dataclass
class SoilParameters:
    """Soil parameter container.
    
    Attributes:
        soil_name: Name of soil type.
        clay: Clay content (%).
        silt: Silt content (%).
        sand: Sand content (%).
        organic_matter: Organic matter content (%).
        bulk_density: Bulk density (g/cm3).
        cn: Curve number.
        layers: List of soil layers.
    """
    
    soil_name: str = "Unknown"
    clay: float = 30.0
    silt: float = 40.0
    sand: float = 30.0
    organic_matter: float = 2.5
    bulk_density: float = 1.3
    cn: int = 70
    layers: List[SoilLayer] = field(default_factory=list)
    
    # Derived properties
    @property
    def field_capacity(self) -> float:
        """Get field capacity from first layer."""
        return self.layers[0].fc if self.layers else 0.32
    
    @property
    def wilting_point(self) -> float:
        """Get wilting point from first layer."""
        return self.layers[0].wp if self.layers else 0.15
    
    @property
    def saturation(self) -> float:
        """Get saturation from first layer."""
        return self.layers[0].saturation if self.layers else 0.45
    
    def to_dict(self) -> Dict[str, float]:
        """Convert to dictionary for C++ API."""
        return {
            "field_capacity": self.field_capacity,
            "wilting_point": self.wilting_point,
            "saturation": self.saturation,
            "cn": float(self.cn),
        }
    
    def load_from_file(self, filename: str) -> None:
        """Load parameters from soil file.
        
        Args:
            filename: Path to soil file (.sol).
        """
        # This is a simplified implementation
        # Full implementation would parse actual .sol format
        if not os.path.exists(filename):
            raise FileNotFoundError(f"Soil file not found: {filename}")
        
        # For now, set some defaults
        self.soil_name = os.path.basename(filename).replace(".sol", "")
        self.layers = [
            SoilLayer(thickness=30, fc=0.32, wp=0.15, saturation=0.45),
            SoilLayer(thickness=70, fc=0.35, wp=0.18, saturation=0.48),
        ]
    
    def set_parameter(self, name: str, value: float) -> None:
        """Set a parameter value.
        
        Args:
            name: Parameter name.
            value: Parameter value.
        """
        if hasattr(self, name):
            setattr(self, name, value)
    
    def add_layer(self, layer: SoilLayer) -> None:
        """Add a soil layer.
        
        Args:
            layer: SoilLayer to add.
        """
        self.layers.append(layer)
    
    def clear_layers(self) -> None:
        """Remove all soil layers."""
        self.layers.clear()


@dataclass
class ClimateParameters:
    """Climate data container.
    
    Attributes:
        tmin_file: Path to minimum temperature file.
        tmax_file: Path to maximum temperature file.
        eto_file: Path to reference ET file.
        rainfall_file: Path to rainfall file.
        co2_file: Path to CO2 concentration file.
    """
    
    tmin_file: Optional[str] = None
    tmax_file: Optional[str] = None
    eto_file: Optional[str] = None
    rainfall_file: Optional[str] = None
    co2_file: Optional[str] = None
    
    def to_dict(self) -> Dict[str, str]:
        """Convert to dictionary for C++ API."""
        return {
            "temperature_min": self.tmin_file or "",
            "temperature_max": self.tmax_file or "",
            "eto": self.eto_file or "",
            "rainfall": self.rainfall_file or "",
        }
    
    def load_from_file(self, filename: str) -> None:
        """Load climate data from file.
        
        Args:
            filename: Path to climate file.
        """
        if not os.path.exists(filename):
            raise FileNotFoundError(f"Climate file not found: {filename}")
        
        # Determine file type from extension
        ext = os.path.splitext(filename)[1].lower()
        
        if ext == '.csv':
            self._load_csv_climate(filename)
        else:
            # Try to infer file types
            basename = os.path.basename(filename).lower()
            if 'tmin' in basename:
                self.tmin_file = filename
            elif 'tmax' in basename:
                self.tmax_file = filename
            elif 'eto' in basename:
                self.eto_file = filename
            elif 'rain' in basename:
                self.rainfall_file = filename
    
    def _load_csv_climate(self, filename: str) -> None:
        """Load climate data from CSV file.
        
        Args:
            filename: Path to CSV file.
        """
        # Simplified implementation
        pass


@dataclass
class CropParameters:
    """Crop parameter container.
    
    Attributes:
        crop_name: Name of crop.
        planting_method: 'seed', 'transplant', or 'regrowth'.
        row_spacing: Row spacing (cm).
        plant_density: Plant density (plants/m2).
        gdd_method: 'calendar' or 'gddays'.
        cycle_length: Crop cycle length (days).
        harvest_index: Harvest index.
        canopy_growth_coeff: Canopy growth coefficient.
        canopy_decline_coeff: Canopy decline coefficient.
    """
    
    crop_name: str = "Unknown"
    planting_method: str = "seed"
    row_spacing: float = 15.0
    plant_density: float = 250.0
    gdd_method: str = "calendar"
    cycle_length: int = 120
    harvest_index: float = 0.45
    canopy_growth_coeff: float = 0.012
    canopy_decline_coeff: float = 0.003
    
    # Water stress parameters
    kc_factor: float = 1.0
    drought_sensitivity: float = 0.5
    
    def to_dict(self) -> Dict:
        """Convert to dictionary for C++ API."""
        return {
            "crop_name": self.crop_name,
            "planting_method": self.planting_method,
            "gdd_method": self.gdd_method,
            "harvest_index": self.harvest_index,
            "cgc": self.canopy_growth_coeff,
            "cdc": self.canopy_decline_coeff,
            "cycle_length": self.cycle_length,
        }
    
    def load_from_file(self, filename: str) -> None:
        """Load parameters from crop file.
        
        Args:
            filename: Path to crop file (.Crop).
        """
        if not os.path.exists(filename):
            raise FileNotFoundError(f"Crop file not found: {filename}")
        
        # Simplified: set crop name from filename
        self.crop_name = os.path.basename(filename).replace(".Crop", "").replace(".crop", "")
    
    def set_parameter(self, name: str, value) -> None:
        """Set a parameter value.
        
        Args:
            name: Parameter name.
            value: Parameter value.
        """
        if hasattr(self, name):
            setattr(self, name, value)


@dataclass
class IrrigationParameters:
    """Irrigation parameter container.
    
    Attributes:
        method: 'no_irri', 'manual', 'scheduled', 'generate', or 'automatic'.
        amount: Fixed irrigation amount (mm).
        schedule: List of (day, amount) tuples.
        efficiency: Irrigation efficiency (0-1).
    """
    
    method: str = "no_irri"
    amount: float = 0.0
    schedule: List[tuple] = field(default_factory=list)
    efficiency: float = 1.0
    
    def to_dict(self) -> Dict:
        """Convert to dictionary for C++ API."""
        return {
            "method": self.method,
            "amount": self.amount,
            "schedule": self.schedule,
            "efficiency": self.efficiency,
        }
    
    def from_dict(self, params: Dict) -> None:
        """Set parameters from dictionary.
        
        Args:
            params: Dictionary of irrigation parameters.
        """
        if 'method' in params:
            self.method = params['method']
        if 'amount' in params:
            self.amount = params['amount']
        if 'schedule' in params:
            self.schedule = params['schedule']
        if 'efficiency' in params:
            self.efficiency = params['efficiency']
    
    def add_application(self, day: int, amount: float) -> None:
        """Add an irrigation application.
        
        Args:
            day: Day of simulation.
            amount: Irrigation amount (mm).
        """
        self.schedule.append((day, amount))
        self.schedule.sort(key=lambda x: x[0])
    
    def clear_schedule(self) -> None:
        """Remove all irrigation applications."""
        self.schedule.clear()


@dataclass
class SimulationConfig:
    """Simulation configuration container.
    
    Attributes:
        start_date: Simulation start date.
        end_date: Simulation end date.
        initial_swc: Initial soil water content (fraction).
        initial_root_depth: Initial root depth (m).
        multiple_runs: Enable multiple runs mode.
        output_dir: Output directory path.
        aggregation_interval: Temporal aggregation interval (days).
    """
    
    start_date: Optional[datetime] = None
    end_date: Optional[datetime] = None
    initial_swc: float = 0.5
    initial_root_depth: float = 0.1
    multiple_runs: bool = False
    output_dir: str = "OUTP"
    aggregation_interval: int = 1
    
    def to_dict(self) -> Dict:
        """Convert to dictionary."""
        result = {
            "initial_swc": self.initial_swc,
            "initial_root_depth": self.initial_root_depth,
            "multiple_runs": self.multiple_runs,
            "output_dir": self.output_dir,
            "aggregation_interval": self.aggregation_interval,
        }
        
        if self.start_date:
            result["start_date"] = self.start_date.strftime("%Y-%m-%d")
        if self.end_date:
            result["end_date"] = self.end_date.strftime("%Y-%m-%d")
        
        return result
