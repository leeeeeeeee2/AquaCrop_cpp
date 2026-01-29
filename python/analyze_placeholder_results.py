#!/usr/bin/env python3
"""
Analyze placeholder AquaCrop simulation results.
This script handles the placeholder output format and provides analysis.
"""

import sys
import re
from pathlib import Path
from typing import Dict, List, Tuple


class PlaceholderResultAnalyzer:
    """Analyze placeholder AquaCrop results."""
    
    def __init__(self, result_file: Path):
        """Initialize the analyzer.
        
        Args:
            result_file: Path to result.txt file.
        """
        self.result_file = Path(result_file)
        self.daily_data = []
        self.seasonal_data = {}
        
    def parse_results(self) -> bool:
        """Parse placeholder results.
        
        Returns:
            True if parsing succeeded, False otherwise.
        """
        if not self.result_file.exists():
            print(f"Error: Result file not found: {self.result_file}")
            return False
        
        with open(self.result_file, "r") as f:
            content = f.read()
            
            # Check if this is placeholder output
            if "SIMULATED AquaCrop run (placeholder)" not in content:
                print("Warning: Not a placeholder result file")
                return False
            
            # Parse daily data
            pattern = r"Day (\d+): biomass=([\d.]+), canopy=([\d.]+), transpiration=([\d.]+), soil_moisture=([\d.]+)"
            matches = re.findall(pattern, content)
            
            for match in matches:
                day = int(match[0])
                biomass = float(match[1])
                canopy = float(match[2])
                transpiration = float(match[3])
                soil_moisture = float(match[4])
                
                self.daily_data.append({
                    "day": day,
                    "biomass": biomass,
                    "canopy": canopy,
                    "transpiration": transpiration,
                    "soil_moisture": soil_moisture,
                })
            
            print(f"Parsed {len(self.daily_data)} days of simulation data")
            return len(self.daily_data) > 0
    
    def calculate_seasonal_totals(self) -> Dict[str, float]:
        """Calculate seasonal totals and averages.
        
        Returns:
            Dictionary with seasonal statistics.
        """
        if not self.daily_data:
            return {}
        
        totals = {
            "simulation_days": len(self.daily_data),
            "total_transpiration": sum(d["transpiration"] for d in self.daily_data),
            "max_biomass": max(d["biomass"] for d in self.daily_data),
            "max_canopy": max(d["canopy"] for d in self.daily_data),
            "final_biomass": self.daily_data[-1]["biomass"] if self.daily_data else 0,
            "final_canopy": self.daily_data[-1]["canopy"] if self.daily_data else 0,
            "avg_transpiration": sum(d["transpiration"] for d in self.daily_data) / len(self.daily_data),
            "avg_canopy": sum(d["canopy"] for d in self.daily_data) / len(self.daily_data),
        }
        
        self.seasonal_data = totals
        return totals
    
    def print_summary(self) -> None:
        """Print analysis summary."""
        if not self.seasonal_data:
            self.calculate_seasonal_totals()
        
        s = self.seasonal_data
        
        print("\n" + "=" * 70)
        print("PLACEHOLDER AQUACROP SIMULATION ANALYSIS")
        print("=" * 70)
        print(f"Result file: {self.result_file}")
        print()
        print("Simulation Period:")
        print(f"  Days simulated: {s['simulation_days']}")
        print()
        print("Crop Growth:")
        print(f"  Final biomass:     {s['final_biomass']:.1f} kg/ha")
        print(f"  Maximum biomass:   {s['max_biomass']:.1f} kg/ha")
        print(f"  Final canopy:      {s['final_canopy']:.1f} %")
        print(f"  Maximum canopy:    {s['max_canopy']:.1f} %")
        print()
        print("Water Balance:")
        print(f"  Total transpiration: {s['total_transpiration']:.1f} mm")
        print(f"  Avg transpiration:  {s['avg_transpiration']:.2f} mm/day")
        print(f"  Avg canopy cover:   {s['avg_canopy']:.1f} %")
        print()
        print("Growth Trends:")
        if len(self.daily_data) >= 2:
            biomass_growth_rate = (self.daily_data[-1]["biomass"] - self.daily_data[0]["biomass"]) / len(self.daily_data)
            canopy_growth_rate = (self.daily_data[-1]["canopy"] - self.daily_data[0]["canopy"]) / len(self.daily_data)
            print(f"  Biomass growth rate: {biomass_growth_rate:.2f} kg/ha/day")
            print(f"  Canopy growth rate:  {canopy_growth_rate:.2f} %/day")
        print()
        
        print("Note: These are PLACEHOLDER results. The C++ simulation")
        print("      is using simulated data until full implementation")
        print("      of the AquaCrop algorithms is complete.")
        print("=" * 70)
    
    def print_daily_data(self) -> None:
        """Print daily data table."""
        if not self.daily_data:
            return
        
        print("\n" + "=" * 70)
        print("DAILY SIMULATION DATA")
        print("=" * 70)
        print(f"{'Day':>4s}  {'Biomass':>10s}  {'Canopy':>8s}  {'Transp':>8s}  {'Soil Moisture':>12s}")
        print("-" * 70)
        
        for d in self.daily_data:
            print(f"{d['day']:>4d}  {d['biomass']:>10.1f}  {d['canopy']:>8.1f}  "
                  f"{d['transpiration']:>8.2f}  {d['soil_moisture']:>12.1f}")
        
        print("=" * 70)
    
    def export_to_csv(self, output_file: Path) -> None:
        """Export results to CSV file.
        
        Args:
            output_file: Output file path.
        """
        with open(output_file, "w") as f:
            f.write("Day,Biomass(kg/ha),Canopy(%),Transpiration(mm),SoilMoisture(%)\n")
            for d in self.daily_data:
                f.write(f"{d['day']},{d['biomass']},{d['canopy']},"
                        f"{d['transpiration']},{d['soil_moisture']}\n")
        
        print(f"Exported results to: {output_file}")


def main():
    """Main entry point."""
    if len(sys.argv) < 2:
        print("Usage: python analyze_placeholder_results.py <result_file>")
        sys.exit(1)
    
    result_file = Path(sys.argv[1])
    
    analyzer = PlaceholderResultAnalyzer(result_file)
    
    if not analyzer.parse_results():
        sys.exit(1)
    
    analyzer.calculate_seasonal_totals()
    analyzer.print_summary()
    analyzer.print_daily_data()
    
    # Export to CSV
    csv_file = result_file.parent / f"{result_file.stem}_parsed.csv"
    analyzer.export_to_csv(csv_file)


if __name__ == "__main__":
    main()
