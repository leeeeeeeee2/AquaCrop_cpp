"""
Results classes for AquaCrop Python wrapper.

This module provides classes for accessing and exporting simulation results.
"""

from typing import Dict, List, Optional, Union
from dataclasses import dataclass
import json
import csv
import os


@dataclass
class DailyResults:
    """Container for daily simulation results.
    
    Attributes:
        date: Simulation date.
        day: Day number.
        gdd: Growing degree days.
        cc: Canopy cover (fraction).
        taw: Total available water (mm).
        dr: Root zone depletion (mm).
        et: Actual evapotranspiration (mm).
        eto: Reference ET (mm).
        kc: Crop coefficient.
        biomass: Cumulative biomass (kg/ha).
        yield_: Yield (kg/ha).
    """
    
    date: str = ""
    day: int = 0
    gdd: float = 0.0
    cc: float = 0.0
    taw: float = 0.0
    dr: float = 0.0
    et: float = 0.0
    eto: float = 0.0
    kc: float = 0.0
    biomass: float = 0.0
    yield_: float = 0.0


@dataclass
class SeasonalSummary:
    """Container for seasonal summary statistics.
    
    Attributes:
        total_rainfall: Total rainfall (mm).
        total_irrigation: Total irrigation (mm).
        total_et: Total evapotranspiration (mm).
        total_biomass: Total biomass (kg/ha).
        grain_yield: Grain yield (kg/ha).
        harvest_index: Harvest index.
        water_productivity: Water productivity (kg/m3).
        simulation_days: Number of simulation days.
    """
    
    total_rainfall: float = 0.0
    total_irrigation: float = 0.0
    total_et: float = 0.0
    total_biomass: float = 0.0
    grain_yield: float = 0.0
    harvest_index: float = 0.0
    water_productivity: float = 0.0
    simulation_days: int = 0
    
    def to_dict(self) -> Dict[str, float]:
        """Convert to dictionary."""
        return {
            "total_rainfall_mm": self.total_rainfall,
            "total_irrigation_mm": self.total_irrigation,
            "total_et_mm": self.total_et,
            "total_biomass_kg_ha": self.total_biomass,
            "yield_kg_ha": self.grain_yield,
            "harvest_index": self.harvest_index,
            "water_productivity_kg_m3": self.water_productivity,
            "simulation_days": self.simulation_days,
        }


class Results:
    """Simulation results container.
    
    This class provides access to simulation results and methods
    for exporting results to various formats.
    
    Attributes:
        daily: List of daily results.
        seasonal: Seasonal summary.
    """
    
    def __init__(self):
        """Initialize results container."""
        self.daily: List[DailyResults] = []
        self.seasonal: SeasonalSummary = SeasonalSummary()
    
    def daily_dataframe(self):
        """Get daily results as pandas DataFrame.
        
        Returns:
            pandas DataFrame with daily results.
        """
        try:
            import pandas as pd
            
            data = []
            for d in self.daily:
                data.append({
                    "date": d.date,
                    "day": d.day,
                    "gdd": d.gdd,
                    "cc": d.cc,
                    "taw": d.taw,
                    "dr": d.dr,
                    "et": d.et,
                    "eto": d.eto,
                    "kc": d.kc,
                    "biomass": d.biomass,
                    "yield": d.yield_,
                })
            
            return pd.DataFrame(data)
        
        except ImportError:
            raise ImportError("pandas is required for DataFrame support")
    
    def seasonal(self) -> Dict[str, float]:
        """Get seasonal summary.
        
        Returns:
            Dictionary with seasonal totals and averages.
        """
        return self.seasonal.to_dict()
    
    def summary(self) -> str:
        """Get formatted summary string.
        
        Returns:
            Human-readable summary of results.
        """
        s = self.seasonal
        lines = [
            "Seasonal Summary:",
            f"  Total rainfall: {s.total_rainfall:.1f} mm",
            f"  Total irrigation: {s.total_irrigation:.1f} mm",
            f"  Total ET: {s.total_et:.1f} mm",
            f"  Total biomass: {s.total_biomass:.1f} kg/ha",
            f"  Grain yield: {s.grain_yield:.1f} kg/ha",
            f"  Harvest index: {s.harvest_index:.3f}",
            f"  Water productivity: {s.water_productivity:.3f} kg/m3",
            f"  Simulation days: {s.simulation_days}",
        ]
        return "\n".join(lines)
    
    def export(self, filename: str, format: str = "csv") -> None:
        """Export results to file.
        
        Args:
            filename: Output file path.
            format: 'csv' or 'json'.
        """
        if format == "csv":
            self.export_csv(filename)
        elif format == "json":
            self.export_json(filename)
        else:
            raise ValueError(f"Unknown format: {format}. Use 'csv' or 'json'")
    
    def export_csv(self, filename: str) -> None:
        """Export daily results to CSV.
        
        Args:
            filename: Output file path.
        """
        with open(filename, 'w', newline='') as f:
            writer = csv.writer(f)
            
            # Write header
            writer.writerow([
                "date", "day", "gdd", "cc", "taw", "dr",
                "et", "eto", "kc", "biomass", "yield"
            ])
            
            # Write data
            for d in self.daily:
                writer.writerow([
                    d.date, d.day, d.gdd, d.cc, d.taw, d.dr,
                    d.et, d.eto, d.kc, d.biomass, d.yield_
                ])
    
    def export_json(self, filename: str) -> None:
        """Export results to JSON.
        
        Args:
            filename: Output file path.
        """
        data = {
            "seasonal": self.seasonal.to_dict(),
            "daily": []
        }
        
        for d in self.daily:
            data["daily"].append({
                "date": d.date,
                "day": d.day,
                "gdd": d.gdd,
                "cc": d.cc,
                "taw": d.taw,
                "dr": d.dr,
                "et": d.et,
                "eto": d.eto,
                "kc": d.kc,
                "biomass": d.biomass,
                "yield": d.yield_,
            })
        
        with open(filename, 'w') as f:
            json.dump(data, f, indent=2)
    
    def export_seasonal_csv(self, filename: str) -> None:
        """Export seasonal summary to CSV.
        
        Args:
            filename: Output file path.
        """
        with open(filename, 'w', newline='') as f:
            writer = csv.writer(f)
            
            # Write header
            writer.writerow(["Parameter", "Value", "Unit"])
            
            # Write data
            for key, value in self.seasonal.to_dict().items():
                unit = ""
                if "mm" in key:
                    unit = "mm"
                elif "kg_ha" in key:
                    unit = "kg/ha"
                elif "kg_m3" in key:
                    unit = "kg/m3"
                elif "index" in key or "days" in key:
                    unit = ""
                
                writer.writerow([key, value, unit])
    
    def load(self, filename: str) -> None:
        """Load results from file.
        
        Args:
            filename: Path to results file.
        """
        if not os.path.exists(filename):
            raise FileNotFoundError(f"Results file not found: {filename}")
        
        # Determine format from extension
        ext = os.path.splitext(filename)[1].lower()
        
        if ext == '.json':
            self._load_json(filename)
        elif ext == '.csv':
            self._load_csv(filename)
        else:
            raise ValueError(f"Unknown file format: {ext}")
    
    def _load_json(self, filename: str) -> None:
        """Load results from JSON file.
        
        Args:
            filename: Path to JSON file.
        """
        with open(filename, 'r') as f:
            data = json.load(f)
        
        # Load seasonal
        if "seasonal" in data:
            s_data = data["seasonal"]
            self.seasonal = SeasonalSummary(
                total_rainfall=s_data.get("total_rainfall_mm", 0.0),
                total_irrigation=s_data.get("total_irrigation_mm", 0.0),
                total_et=s_data.get("total_et_mm", 0.0),
                total_biomass=s_data.get("total_biomass_kg_ha", 0.0),
                grain_yield=s_data.get("yield_kg_ha", 0.0),
                harvest_index=s_data.get("harvest_index", 0.0),
                water_productivity=s_data.get("water_productivity_kg_m3", 0.0),
            )
        
        # Load daily
        if "daily" in data:
            self.daily = []
            for d_data in data["daily"]:
                daily = DailyResults(
                    date=d_data.get("date", ""),
                    day=d_data.get("day", 0),
                    gdd=d_data.get("gdd", 0.0),
                    cc=d_data.get("cc", 0.0),
                    taw=d_data.get("taw", 0.0),
                    dr=d_data.get("dr", 0.0),
                    et=d_data.get("et", 0.0),
                    eto=d_data.get("eto", 0.0),
                    kc=d_data.get("kc", 0.0),
                    biomass=d_data.get("biomass", 0.0),
                    yield_=d_data.get("yield", 0.0),
                )
                self.daily.append(daily)
    
    def _load_csv(self, filename: str) -> None:
        """Load results from CSV file.
        
        Args:
            filename: Path to CSV file.
        """
        with open(filename, 'r') as f:
            reader = csv.DictReader(f)
            
            self.daily = []
            for row in reader:
                daily = DailyResults(
                    date=row.get("date", ""),
                    day=int(row.get("day", 0)),
                    gdd=float(row.get("gdd", 0.0)),
                    cc=float(row.get("cc", 0.0)),
                    taw=float(row.get("taw", 0.0)),
                    dr=float(row.get("dr", 0.0)),
                    et=float(row.get("et", 0.0)),
                    eto=float(row.get("eto", 0.0)),
                    kc=float(row.get("kc", 0.0)),
                    biomass=float(row.get("biomass", 0.0)),
                    yield_=float(row.get("yield", 0.0)),
                )
                self.daily.append(daily)
    
    def get_time_series(self, variable: str) -> List[float]:
        """Get a specific variable as a time series.
        
        Args:
            variable: Name of variable ('cc', 'biomass', 'yield', 'et').
        
        Returns:
            List of values.
        """
        attr_map = {
            "cc": "cc",
            "biomass": "biomass",
            "yield": "yield_",
            "et": "et",
            "eto": "eto",
            "kc": "kc",
            "gdd": "gdd",
            "taw": "taw",
            "dr": "dr",
        }
        
        if variable not in attr_map:
            raise ValueError(f"Unknown variable: {variable}. Choose from: {list(attr_map.keys())}")
        
        attr = attr_map[variable]
        return [getattr(d, attr) for d in self.daily]
    
    def calculate_statistics(self, variable: str) -> Dict[str, float]:
        """Calculate statistics for a variable.
        
        Args:
            variable: Name of variable.
        
        Returns:
            Dictionary with statistics (mean, std, min, max, sum).
        """
        import math
        
        values = self.get_time_series(variable)
        
        if not values:
            return {"mean": 0, "std": 0, "min": 0, "max": 0, "sum": 0}
        
        n = len(values)
        sum_val = sum(values)
        mean_val = sum_val / n
        
        # Standard deviation
        var = sum((x - mean_val) ** 2 for x in values) / n
        std_val = math.sqrt(var)
        
        return {
            "mean": mean_val,
            "std": std_val,
            "min": min(values),
            "max": max(values),
            "sum": sum_val,
            "n": n,
        }
