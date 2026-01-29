"""
Model classes and functions for AquaCrop Python wrapper.

This module provides the main Model class for configuring and running
AquaCrop simulations.
"""

from typing import Optional, Dict, List, Union
from dataclasses import dataclass, field
import os
from datetime import datetime

from . import _core
from .parameters import (
    SoilParameters,
    ClimateParameters,
    CropParameters,
    IrrigationParameters,
    SimulationConfig,
)
from .results import Results


@dataclass
class Model:
    """AquaCrop simulation model.
    
    This class provides the main interface for configuring and running
    AquaCrop simulations from Python.
    
    Attributes:
        config: Simulation configuration object.
        soil_params: Soil parameters.
        climate_params: Climate parameters.
        crop_params: Crop parameters.
        irrigation_params: Irrigation parameters.
    """
    
    config: SimulationConfig = field(default_factory=SimulationConfig)
    soil_params: SoilParameters = field(default_factory=SoilParameters)
    climate_params: ClimateParameters = field(default_factory=ClimateParameters)
    crop_params: CropParameters = field(default_factory=CropParameters)
    irrigation_params: IrrigationParameters = field(default_factory=IrrigationParameters)
    
    def __post_init__(self):
        """Initialize model after construction."""
        pass
    
    # Soil methods
    def set_soil(self, soil_file: str) -> None:
        """Set soil parameters from file.
        
        Args:
            soil_file: Path to soil parameter file (.sol).
        """
        if not os.path.exists(soil_file):
            raise FileNotFoundError(f"Soil file not found: {soil_file}")
        self.soil_params.load_from_file(soil_file)
    
    def set_soil_parameters(self, params: Dict[str, float]) -> None:
        """Set soil parameters directly.
        
        Args:
            params: Dictionary of soil parameters.
                - field_capacity: Field capacity (fraction)
                - wilting_point: Wilting point (fraction)
                - saturation: Saturation point (fraction)
                - cn: Curve number
        """
        for key, value in params.items():
            if hasattr(self.soil_params, key):
                setattr(self.soil_params, key, value)
    
    # Climate methods
    def set_climate(self, climate_file: str) -> None:
        """Set climate data from file.
        
        Args:
            climate_file: Path to climate file.
        """
        if not os.path.exists(climate_file):
            raise FileNotFoundError(f"Climate file not found: {climate_file}")
        self.climate_params.load_from_file(climate_file)
    
    def set_climate_parameters(self, params: Dict[str, str]) -> None:
        """Set climate file paths.
        
        Args:
            params: Dictionary of climate file paths:
                - temperature_min: Tmin file path
                - temperature_max: Tmax file path
                - eto: ETo file path
                - rainfall: Rainfall file path
        """
        for key, value in params.items():
            if hasattr(self.climate_params, key):
                setattr(self.climate_params, key, value)
    
    # Crop methods
    def set_crop(self, crop_file: str) -> None:
        """Set crop parameters from file.
        
        Args:
            crop_file: Path to crop parameter file (.Crop).
        """
        if not os.path.exists(crop_file):
            raise FileNotFoundError(f"Crop file not found: {crop_file}")
        self.crop_params.load_from_file(crop_file)
    
    def set_crop_parameters(self, params: Dict) -> None:
        """Set crop parameters directly.
        
        Args:
            params: Dictionary of crop parameters.
        """
        for key, value in params.items():
            if hasattr(self.crop_params, key):
                setattr(self.crop_params, key, value)
    
    # Irrigation methods
    def set_irrigation(self, irrigation: Dict) -> None:
        """Set irrigation parameters.
        
        Args:
            irrigation: Dictionary with:
                - method: 'manual', 'scheduled', or 'automatic'
                - amount: Irrigation amount (mm)
                - schedule: List of (day, amount) tuples
        """
        self.irrigation_params.from_dict(irrigation)
    
    # Simulation control
    def set_start_date(self, date: Union[str, datetime]) -> None:
        """Set simulation start date.
        
        Args:
            date: Start date in YYYY-MM-DD format or datetime object.
        """
        if isinstance(date, str):
            date = datetime.strptime(date, "%Y-%m-%d")
        self.config.start_date = date
    
    def set_end_date(self, date: Union[str, datetime]) -> None:
        """Set simulation end date.
        
        Args:
            date: End date in YYYY-MM-DD format or datetime object.
        """
        if isinstance(date, str):
            date = datetime.strptime(date, "%Y-%m-%d")
        self.config.end_date = date
    
    def set_initial_conditions(self, conditions: Dict) -> None:
        """Set initial soil water conditions.
        
        Args:
            conditions: Dictionary with:
                - swc: Initial soil water content (fraction)
                - depth: Initial root depth (m)
        """
        if 'swc' in conditions:
            self.config.initial_swc = conditions['swc']
        if 'depth' in conditions:
            self.config.initial_root_depth = conditions['depth']
    
    def set_multiple_runs(self, enable: bool = True) -> None:
        """Enable or disable multiple runs mode.
        
        Args:
            enable: True to enable multiple runs.
        """
        self.config.multiple_runs = enable
    
    def set_output_directory(self, directory: str) -> None:
        """Set output directory for results.
        
        Args:
            directory: Path to output directory.
        """
        self.config.output_dir = directory
    
    def set_temporal_aggregation(self, interval: int) -> None:
        """Set temporal aggregation interval for output.
        
        Args:
            interval: Aggregation interval in days (1 = daily).
        """
        self.config.aggregation_interval = interval
    
    # Run simulation
    def run(self, project_file: Optional[str] = None) -> None:
        """Run the simulation.
        
        Args:
            project_file: Optional path to project file. If provided,
                all other settings are loaded from this file.
        """
        # If project file is provided, use it
        if project_file is not None:
            if not os.path.exists(project_file):
                raise FileNotFoundError(f"Project file not found: {project_file}")
            # Create C++ model and run project
            model = _core.Model()
            model.run(project_file)
            return
        
        # Otherwise, configure and run
        # Convert parameters to C++ format
        soil_dict = self.soil_params.to_dict()
        climate_dict = self.climate_params.to_dict()
        crop_dict = self.crop_params.to_dict()
        irrigation_dict = self.irrigation_params.to_dict()
        
        # Create C++ model
        model = _core.Model()
        
        # Configure model
        if soil_dict:
            model.set_soil_parameters(soil_dict)
        if climate_dict:
            model.set_climate_parameters(climate_dict)
        if crop_dict:
            model.set_crop_parameters(crop_dict)
        if irrigation_dict:
            model.set_irrigation(irrigation_dict)
        
        # Set dates
        if self.config.start_date:
            model.set_start_date(self.config.start_date.strftime("%Y-%m-%d"))
        if self.config.end_date:
            model.set_end_date(self.config.end_date.strftime("%Y-%m-%d"))
        
        # Run simulation
        model.run()
    
    def run_parameter_study(
        self,
        param_name: str,
        values: List[float],
        output_prefix: Optional[str] = None
    ) -> List[Results]:
        """Run simulation for multiple parameter values.
        
        Args:
            param_name: Name of parameter to vary.
            values: List of parameter values to test.
            output_prefix: Optional prefix for output files.
        
        Returns:
            List of Results objects for each simulation.
        """
        results = []
        original_value = None
        
        # Store original value
        if hasattr(self.soil_params, param_name):
            original_value = getattr(self.soil_params, param_name)
            param_type = "soil"
        elif hasattr(self.crop_params, param_name):
            original_value = getattr(self.crop_params, param_name)
            param_type = "crop"
        else:
            raise ValueError(f"Unknown parameter: {param_name}")
        
        # Run simulations
        for i, value in enumerate(values):
            # Set parameter value
            if param_type == "soil":
                self.soil_params.set_parameter(param_name, value)
            else:
                self.crop_params.set_parameter(param_name, value)
            
            # Run simulation
            self.run()
            
            # Get results
            result = self.get_results()
            results.append(result)
            
            # Save output if requested
            if output_prefix:
                filename = f"{output_prefix}_{i}.csv"
                result.export_csv(filename)
        
        # Restore original value
        if param_type == "soil":
            self.soil_params.set_parameter(param_name, original_value)
        else:
            self.crop_params.set_parameter(param_name, original_value)
        
        return results
    
    # Results
    def get_results(self) -> Results:
        """Get simulation results.
        
        Returns:
            Results object containing all output data.
        """
        # This would typically get results from the C++ model
        # For now, return an empty Results object
        return Results()
    
    def load_results(self, filename: str) -> Results:
        """Load results from file.
        
        Args:
            filename: Path to results file.
        
        Returns:
            Results object loaded from file.
        """
        results = Results()
        results.load(filename)
        return results


# Convenience function for quick simulations
def quick_run(
    soil: Optional[str] = None,
    climate: Optional[str] = None,
    crop: Optional[str] = None,
    start_date: Optional[str] = None,
    end_date: Optional[str] = None,
    irrigation_amount: float = 0.0
) -> Results:
    """Quickly run a simulation with minimal configuration.
    
    Args:
        soil: Path to soil file.
        climate: Path to climate file or dict of climate files.
        crop: Path to crop file.
        start_date: Start date (YYYY-MM-DD).
        end_date: End date (YYYY-MM-DD).
        irrigation_amount: Fixed irrigation amount (mm).
    
    Returns:
        Results object.
    """
    model = Model()
    
    if soil:
        model.set_soil(soil)
    if climate:
        if isinstance(climate, dict):
            model.set_climate_parameters(climate)
        else:
            model.set_climate(climate)
    if crop:
        model.set_crop(crop)
    if start_date:
        model.set_start_date(start_date)
    if end_date:
        model.set_end_date(end_date)
    if irrigation_amount > 0:
        model.set_irrigation({
            "method": "manual",
            "amount": irrigation_amount
        })
    
    model.run()
    
    return model.get_results()
