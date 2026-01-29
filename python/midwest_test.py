#!/usr/bin/env python3
"""
Generate a U.S. Midwest corn test case for AquaCrop.

This script creates a realistic test case representing corn production
in the U.S. Midwest (Iowa region), including:
- Climate data (temperature, rainfall, ET, CO2)
- Soil profile (typical Midwestern soil)
- Crop parameters (corn/maize)
- Management practices
"""

import sys
from pathlib import Path
from datetime import datetime, timedelta
import numpy as np


class MidwestTestCaseGenerator:
    """Generate realistic Midwest corn test case."""
    
    def __init__(self, output_dir: Path):
        """Initialize the generator.
        
        Args:
            output_dir: Output directory for the test case.
        """
        self.output_dir = Path(output_dir)
        self.case_name = "case-midwest-corn"
        self.case_dir = self.output_dir / self.case_name
        
        # Midwest climate parameters (Iowa region)
        # Typical values for corn production
        self.location = {
            "name": "Iowa, USA",
            "latitude": 41.9,
            "longitude": -93.6,
            "elevation": 230,
            "co2_concentration": 410,  # ppm (2024)
        }
        
        # Corn parameters
        self.crop_params = {
            "name": "Maize",
            "type": "C4",
            "planting_method": "Seed",
            "plant_density": 8.0,  # plants/m²
            "row_spacing": 0.76,  # m
            "calendar_type": "CalendarDays",
            "cycle_length": 120,  # days
            "harvest_index": 0.48,  # typical for corn
            "cgc": 0.013,  # canopy growth coefficient
            "cdc": 0.003,  # canopy decline coefficient
            "kc": 1.05,  # crop coefficient
        }
        
        # Soil parameters (typical Midwestern soil - loam)
        self.soil_params = {
            "name": "SiltLoam_Midwest",
            "texture": "Silt Loam",
            "bulk_density": 1.35,  # g/cm³
            "organic_matter": 2.8,  # %
            "saturation": 0.48,
            "field_capacity": 0.32,
            "wilting_point": 0.14,
            "curve_number": 75,
            "layers": [
                {"thickness": 30, "fc": 0.32, "wp": 0.14, "sat": 0.48},
                {"thickness": 40, "fc": 0.34, "wp": 0.16, "sat": 0.50},
                {"thickness": 50, "fc": 0.36, "wp": 0.18, "sat": 0.52},
                {"thickness": 60, "fc": 0.38, "wp": 0.20, "sat": 0.54},
            ],
        }
        
    def generate_climate_data(self, year: int = 2024) -> dict:
        """Generate realistic climate data for Midwest.
        
        Args:
            year: Year to generate data for.
        
        Returns:
            Dictionary with climate data arrays.
        """
        # Planting date: May 1
        planting_date = datetime(year, 5, 1)
        # Harvest date: October 15
        harvest_date = datetime(year, 10, 15)
        
        days = (harvest_date - planting_date).days + 1
        dates = [planting_date + timedelta(days=d) for d in range(days)]
        
        # Generate realistic climate data for Iowa corn season
        # Temperature (°C) - typical pattern
        temp_min = np.zeros(days)
        temp_max = np.zeros(days)
        
        for i, date in enumerate(dates):
            day_of_year = (date - datetime(year, 1, 1)).days
            # Temperature follows seasonal pattern
            base_temp = 15 + 15 * np.sin(2 * np.pi * (day_of_year - 80) / 365)
            # Add daily variation
            temp_min[i] = base_temp - 8 + np.random.normal(0, 2)
            temp_max[i] = base_temp + 8 + np.random.normal(0, 2)
        
        # Rainfall (mm) - typical Midwest pattern with thunderstorms
        # More rain in early summer (June-July)
        rainfall = np.zeros(days)
        for i in range(days):
            if np.random.random() < 0.25:  # 25% chance of rain on any day
                # Amount follows exponential distribution (more light rains, fewer heavy)
                rainfall[i] = np.random.exponential(8)
                # Cap at 50mm (rare extreme events)
                rainfall[i] = min(rainfall[i], 50)
        
        # Reference ET (mm/day) - follows temperature and solar radiation pattern
        eto = np.zeros(days)
        for i, date in enumerate(dates):
            day_of_year = (date - datetime(year, 1, 1)).days
            # Peak ET in July
            base_eto = 3.5 + 3.5 * np.sin(2 * np.pi * (day_of_year - 80) / 365)
            eto[i] = max(1.0, base_eto + np.random.normal(0, 0.5))
        
        # CO2 concentration (ppm) - slowly increasing over time
        co2 = np.full(days, self.location["co2_concentration"])
        
        return {
            "dates": dates,
            "temp_min": temp_min,
            "temp_max": temp_max,
            "rainfall": rainfall,
            "eto": eto,
            "co2": co2,
        }
    
    def generate_case(self) -> Path:
        """Generate the complete test case.
        
        Returns:
            Path to the generated case directory.
        """
        # Create directory structure
        self.case_dir.mkdir(parents=True, exist_ok=True)
        
        subdirs = ["CLIM", "CROP", "SOIL", "IRRIG", "MANAGE", "GWT", "OFF", "OBS", "output", "params"]
        for subdir in subdirs:
            (self.case_dir / subdir).mkdir(exist_ok=True)
        
        # Generate climate data
        climate = self.generate_climate_data(2024)
        
        # Write climate files
        self._write_climate_files(climate)
        
        # Write crop file
        self._write_crop_file()
        
        # Write soil file
        self._write_soil_file()
        
        # Write irrigation file (no irrigation for rainfed)
        self._write_irrigation_file()
        
        # Write management file
        self._write_management_file()
        
        # Write groundwater file
        self._write_groundwater_file()
        
        # Write initial soil water file
        self._write_initial_soil_water()
        
        # Write off-season file
        self._write_offseason_file()
        
        # Write observation file
        self._write_observation_file()
        
        # Write project file
        self._write_project_file()
        
        # Write params.txt
        self._write_params_file()
        
        print(f"Generated Midwest corn test case: {self.case_dir}")
        return self.case_dir
    
    def _write_climate_files(self, climate: dict) -> None:
        """Write climate data files."""
        days = len(climate["dates"])
        
        # Climate.TXT (summary)
        climate_file = self.case_dir / "CLIM" / "climate.TXT"
        with open(climate_file, "w") as f:
            f.write(f"Climate data for {self.case_name}\n")
            f.write(f"{self.location['name']}\n")
            f.write(f"{self.location['latitude']} {self.location['longitude']}\n")
            f.write(f"{self.location['elevation']}\n")
            f.write(f"{self.location['co2_concentration']}\n")
        
        # Temperature.TXT
        temp_file = self.case_dir / "CLIM" / "temperature.TXT"
        with open(temp_file, "w") as f:
            f.write(f"Temperature data for {self.case_name}\n")
            f.write("2\n")  # Number of variables (Tmin, Tmax)
            for i in range(days):
                date_str = climate["dates"][i].strftime("%Y-%m-%d")
                f.write(f"{date_str} {climate['temp_min'][i]:.1f} {climate['temp_max'][i]:.1f}\n")
        
        # Rain.TXT
        rain_file = self.case_dir / "CLIM" / "rain.TXT"
        with open(rain_file, "w") as f:
            f.write(f"Rainfall data for {self.case_name}\n")
            f.write("1\n")  # Number of variables
            for i in range(days):
                date_str = climate["dates"][i].strftime("%Y-%m-%d")
                f.write(f"{date_str} {climate['rainfall'][i]:.1f}\n")
        
        # ETo.TXT
        eto_file = self.case_dir / "CLIM" / "eto.TXT"
        with open(eto_file, "w") as f:
            f.write(f"ET0 data for {self.case_name}\n")
            f.write("1\n")  # Number of variables
            for i in range(days):
                date_str = climate["dates"][i].strftime("%Y-%m-%d")
                f.write(f"{date_str} {climate['eto'][i]:.2f}\n")
        
        # CO2.TXT
        co2_file = self.case_dir / "CLIM" / "co2.TXT"
        with open(co2_file, "w") as f:
            f.write(f"CO2 data for {self.case_name}\n")
            f.write("1\n")
            for i in range(days):
                date_str = climate["dates"][i].strftime("%Y-%m-%d")
                f.write(f"{date_str} {climate['co2'][i]:.1f}\n")
        
        # Calendar.TXT
        calendar_file = self.case_dir / "CLIM" / "calendar.TXT"
        with open(calendar_file, "w") as f:
            f.write(f"Calendar data for {self.case_name}\n")
            f.write("1\n")
            for i, date in enumerate(climate["dates"]):
                f.write(f"{i+1} {date.strftime('%Y-%m-%d')}\n")
    
    def _write_crop_file(self) -> None:
        """Write crop parameter file."""
        crop_file = self.case_dir / "CROP" / "crop.TXT"
        with open(crop_file, "w") as f:
            f.write(f"Crop parameters for {self.case_name}\n")
            f.write(f"{self.crop_params['name']}\n")
            f.write(f"{self.crop_params['type']}\n")
            f.write(f"{self.crop_params['planting_method']}\n")
            f.write(f"{self.crop_params['plant_density']}\n")
            f.write(f"{self.crop_params['row_spacing']}\n")
            f.write(f"{self.crop_params['calendar_type']}\n")
            f.write(f"{self.crop_params['cycle_length']}\n")
            f.write(f"{self.crop_params['harvest_index']}\n")
            f.write(f"{self.crop_params['cgc']}\n")
            f.write(f"{self.crop_params['cdc']}\n")
            f.write(f"{self.crop_params['kc']}\n")
    
    def _write_soil_file(self) -> None:
        """Write soil parameter file."""
        soil_file = self.case_dir / "SOIL" / "soil.TXT"
        with open(soil_file, "w") as f:
            f.write(f"Soil profile for {self.case_name}\n")
            f.write(f"{self.soil_params['name']}\n")
            f.write(f"{self.soil_params['texture']}\n")
            f.write(f"{self.soil_params['bulk_density']}\n")
            f.write(f"{self.soil_params['organic_matter']}\n")
            f.write(f"{self.soil_params['saturation']}\n")
            f.write(f"{self.soil_params['field_capacity']}\n")
            f.write(f"{self.soil_params['wilting_point']}\n")
            f.write(f"{self.soil_params['curve_number']}\n")
            f.write(f"{len(self.soil_params['layers'])}\n")  # Number of layers
            for layer in self.soil_params['layers']:
                f.write(f"{layer['thickness']} {layer['fc']} {layer['wp']} {layer['sat']}\n")
    
    def _write_irrigation_file(self) -> None:
        """Write irrigation file (rainfed = no irrigation)."""
        irrig_file = self.case_dir / "IRRIG" / "irrigation.TXT"
        with open(irrig_file, "w") as f:
            f.write(f"Irrigation schedule for {self.case_name}\n")
            f.write("0\n")  # No irrigation applications (rainfed)
            f.write("1.0\n")  # Irrigation efficiency (not used)
    
    def _write_management_file(self) -> None:
        """Write management file."""
        manage_file = self.case_dir / "MANAGE" / "management.TXT"
        with open(manage_file, "w") as f:
            f.write(f"Management practices for {self.case_name}\n")
            f.write("2024\n")  # Simulation year
            f.write("5\n")  # Planting month
            f.write("1\n")  # Planting day
            f.write("10\n")  # Harvest month
            f.write("15\n")  # Harvest day
            f.write("0\n")  # Tillage type
            f.write("0\n")  # Fertilizer applications
            f.write("0\n")  # Mulch
            f.write("0\n")  # Field bunds
            f.write("0\n")  # Weed control
    
    def _write_groundwater_file(self) -> None:
        """Write groundwater file."""
        gwt_file = self.case_dir / "GWT" / "groundwater.TXT"
        with open(gwt_file, "w") as f:
            f.write(f"Groundwater data for {self.case_name}\n")
            f.write("0\n")  # No groundwater contribution
            f.write("0.0\n")  # Water table depth (not used)
    
    def _write_initial_soil_water(self) -> None:
        """Write initial soil water content file."""
        swc_file = self.case_dir / "SOIL" / "swcini.TXT"
        with open(swc_file, "w") as f:
            f.write(f"Initial soil water content for {self.case_name}\n")
            f.write(f"{len(self.soil_params['layers'])}\n")  # Number of layers
            # Start at field capacity
            for layer in self.soil_params['layers']:
                f.write(f"{layer['fc']}\n")
    
    def _write_offseason_file(self) -> None:
        """Write off-season file."""
        off_file = self.case_dir / "OFF" / "offseason.TXT"
        with open(off_file, "w") as f:
            f.write(f"Off-season data for {self.case_name}\n")
            f.write("0\n")  # No off-season conditions
            f.write("0.0\n")  # Initial soil water
            f.write("0\n")  # No frost days
    
    def _write_observation_file(self) -> None:
        """Write observation file."""
        obs_file = self.case_dir / "OBS" / "observations.TXT"
        with open(obs_file, "w") as f:
            f.write(f"Field observations for {self.case_name}\n")
            f.write("0\n")  # No observations
    
    def _write_project_file(self) -> None:
        """Write project file."""
        project_file = self.case_dir / "project.ACp"
        with open(project_file, "w") as f:
            f.write(f"{self.case_name} - U.S. Midwest Corn Production\n")
            f.write("7.2\n")  # Version
            f.write("1\n")  # Number of simulation years
            f.write("1\n")  # Start year
            f.write("1\n")  # End year
            f.write("1\n")  # Start year (repeat)
            f.write("1\n")  # End year (repeat)
            f.write("1\n")  # Start year (repeat)
            f.write("1\n")  # End year (repeat)
            
            # File definitions
            files = [
                ("Climate", "climate.TXT", "CLIM/"),
                ("Temperature", "temperature.TXT", "CLIM/"),
                ("ET0", "eto.TXT", "CLIM/"),
                ("Rainfall", "rain.TXT", "CLIM/"),
                ("CO2", "co2.TXT", "CLIM/"),
                ("Calendar", "calendar.TXT", "CLIM/"),
                ("Crop", "crop.TXT", "CROP/"),
                ("Irrigation", "irrigation.TXT", "IRRIG/"),
                ("Management", "management.TXT", "MANAGE/"),
                ("Soil", "soil.TXT", "SOIL/"),
                ("Groundwater", "groundwater.TXT", "GWT/"),
                ("InitialSWC", "swcini.TXT", "SOIL/"),
                ("Offseason", "offseason.TXT", "OFF/"),
                ("Observations", "observations.TXT", "OBS/"),
            ]
            
            for desc, filename, subdir in files:
                f.write(f"{desc} data for {self.case_name}\n")
                f.write(f"{filename}\n")
                f.write(f"{subdir}\n")
    
    def _write_params_file(self) -> None:
        """Write params.txt file."""
        params_file = self.case_dir / "params.txt"
        with open(params_file, "w") as f:
            f.write(f"# Parameters for {self.case_name}\n")
            f.write(f"crop_type={self.crop_params['name']}\n")
            f.write(f"planting_date=2024-05-01\n")
            f.write(f"harvest_date=2024-10-15\n")
            f.write(f"location={self.location['name']}\n")
            f.write(f"soil_type={self.soil_params['name']}\n")
            f.write(f"management=rainfed\n")
            f.write(f"created={datetime.now().isoformat()}\n")


def main():
    """Main entry point."""
    import argparse
    
    parser = argparse.ArgumentParser(
        description="Generate U.S. Midwest corn test case"
    )
    parser.add_argument(
        "--output-dir",
        type=str,
        default="PARAM",
        help="Output directory (default: PARAM)"
    )
    parser.add_argument(
        "--year",
        type=int,
        default=2024,
        help="Simulation year (default: 2024)"
    )
    
    args = parser.parse_args()
    
    generator = MidwestTestCaseGenerator(Path(args.output_dir))
    case_dir = generator.generate_case()
    
    print(f"\nTest case generated successfully!")
    print(f"Location: {case_dir}")
    print(f"\nTo run this case:")
    print(f"  1. Add '{generator.case_name}/project.ACp' to LIST/ListProjects.txt")
    print(f"  2. Run: python improved_driver.py run --case {generator.case_name}")


if __name__ == "__main__":
    main()
