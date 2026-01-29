#!/usr/bin/env python3
"""
Analyze AquaCrop simulation results.

This script reads simulation output files and provides detailed analysis
including:
- Seasonal summary statistics
- Daily time series plots
- Water balance components
- Crop growth analysis
"""

import sys
from pathlib import Path
import json
import csv
from typing import Dict, List, Optional
import math


class ResultAnalyzer:
    """Analyze AquaCrop simulation results."""
    
    def __init__(self, case_dir: Path):
        """Initialize the analyzer.
        
        Args:
            case_dir: Path to the case directory.
        """
        self.case_dir = Path(case_dir)
        self.output_dir = self.case_dir / "output"
        self.result_file = self.output_dir / "result.txt"
        
        self.daily_data = []
        self.seasonal_data = {}
        
    def load_results(self) -> bool:
        """Load simulation results.
        
        Returns:
            True if loading succeeded, False otherwise.
        """
        if not self.result_file.exists():
            print(f"Error: Result file not found: {self.result_file}")
            return False
        
        try:
            with open(self.result_file, "r") as f:
                content = f.read()
                
                # Try to parse as CSV or space-delimited
                lines = [line.strip() for line in content.splitlines() if line.strip()]
                
                if len(lines) < 2:
                    print("Error: No data found in result file")
                    return False
                
                # Detect delimiter
                delimiter = "," if "," in lines[1] else None
                
                # Parse header
                if delimiter:
                    headers = [h.strip() for h in lines[0].split(delimiter)]
                    reader = csv.DictReader(lines[1:], fieldnames=headers, delimiter=delimiter)
                else:
                    # Space-delimited
                    headers = lines[0].split()
                    reader = [dict(zip(headers, line.split())) for line in lines[1:]]
                
                # Load daily data
                self.daily_data = []
                for row in reader:
                    row_data = {}
                    for key, value in row.items():
                        try:
                            row_data[key] = float(value)
                        except ValueError:
                            row_data[key] = value
                    self.daily_data.append(row_data)
                
                if not self.daily_data:
                    print("Error: No daily data parsed")
                    return False
                
                print(f"Loaded {len(self.daily_data)} days of simulation data")
                return True
                
        except Exception as e:
            print(f"Error loading results: {e}")
            return False
    
    def calculate_seasonal_totals(self) -> Dict[str, float]:
        """Calculate seasonal totals and averages.
        
        Returns:
            Dictionary with seasonal statistics.
        """
        if not self.daily_data:
            return {}
        
        totals = {
            "total_rainfall": 0.0,
            "total_irrigation": 0.0,
            "total_et": 0.0,
            "total_biomass": 0.0,
            "max_biomass": 0.0,
            "max_canopy": 0.0,
            "final_yield": 0.0,
            "avg_et": 0.0,
            "avg_canopy": 0.0,
        }
        
        for day in self.daily_data:
            if "rain" in day or "Rain" in day:
                rain_val = day.get("rain", day.get("Rain", 0))
                totals["total_rainfall"] += rain_val
            
            if "irrig" in day or "Irrig" in day:
                irrig_val = day.get("irrig", day.get("Irrig", 0))
                totals["total_irrigation"] += irrig_val
            
            if "et" in day or "ET" in day or "Et" in day:
                et_val = day.get("et", day.get("ET", day.get("Et", 0)))
                totals["total_et"] += et_val
            
            if "biomass" in day or "Biomass" in day or "B" in day:
                biomass_val = day.get("biomass", day.get("Biomass", day.get("B", 0)))
                totals["total_biomass"] = max(totals["total_biomass"], biomass_val)
                totals["max_biomass"] = max(totals["max_biomass"], biomass_val)
            
            if "canopy" in day or "Canopy" in day or "CC" in day or "cc" in day:
                canopy_val = day.get("canopy", day.get("Canopy", day.get("CC", day.get("cc", 0))))
                if isinstance(canopy_val, (int, float)):
                    totals["max_canopy"] = max(totals["max_canopy"], canopy_val)
            
            if "yield" in day or "Yield" in day or "Y" in day:
                yield_val = day.get("yield", day.get("Yield", day.get("Y", 0)))
                totals["final_yield"] = yield_val
        
        # Calculate averages
        num_days = len(self.daily_data)
        if num_days > 0:
            totals["avg_et"] = totals["total_et"] / num_days
            if "max_canopy" in totals:
                canopy_values = []
                for day in self.daily_data:
                    if "canopy" in day:
                        val = day.get("canopy", 0)
                        if isinstance(val, (int, float)):
                            canopy_values.append(val)
                if canopy_values:
                    totals["avg_canopy"] = sum(canopy_values) / len(canopy_values)
        
        self.seasonal_data = totals
        return totals
    
    def print_seasonal_summary(self) -> None:
        """Print formatted seasonal summary."""
        if not self.seasonal_data:
            self.calculate_seasonal_totals()
        
        s = self.seasonal_data
        
        print("\n" + "=" * 60)
        print("SEASONAL SUMMARY")
        print("=" * 60)
        print(f"Case: {self.case_dir.name}")
        print(f"Simulation days: {len(self.daily_data)}")
        print()
        print("Water Balance:")
        print(f"  Total rainfall:      {s['total_rainfall']:.1f} mm")
        print(f"  Total irrigation:    {s['total_irrigation']:.1f} mm")
        print(f"  Total ET:            {s['total_et']:.1f} mm")
        print(f"  Average ET:          {s['avg_et']:.2f} mm/day")
        print()
        print("Crop Growth:")
        print(f"  Max biomass:         {s['max_biomass']:.1f} kg/ha")
        print(f"  Max canopy cover:    {s['max_canopy']:.1f} %")
        print(f"  Final yield:         {s['final_yield']:.1f} kg/ha")
        print()
        
        # Calculate water use efficiency
        if s['total_et'] > 0:
            wue = s['final_yield'] / s['total_et']
            print("Water Use Efficiency:")
            print(f"  WUE:                 {wue:.2f} kg/m³")
        
        print("=" * 60)
    
    def print_daily_statistics(self) -> None:
        """Print daily statistics for key variables."""
        if not self.daily_data:
            return
        
        print("\n" + "=" * 60)
        print("DAILY STATISTICS")
        print("=" * 60)
        
        variables = []
        for day in self.daily_data:
            for key in day.keys():
                if key not in variables and isinstance(day[key], (int, float)):
                    variables.append(key)
        
        for var in sorted(variables):
            values = [day[var] for day in self.daily_data if isinstance(day.get(var), (int, float))]
            if values:
                mean_val = sum(values) / len(values)
                std_val = math.sqrt(sum((v - mean_val) ** 2 for v in values) / len(values))
                print(f"{var:20s}: min={min(values):7.2f}, max={max(values):7.2f}, "
                      f"mean={mean_val:7.2f}, std={std_val:7.2f}")
        
        print("=" * 60)
    
    def print_time_series(self, variables: Optional[List[str]] = None) -> None:
        """Print time series for specified variables.
        
        Args:
            variables: List of variable names. If None, prints key variables.
        """
        if not self.daily_data:
            return
        
        if variables is None:
            variables = []
            for day in self.daily_data:
                for key in day.keys():
                    if key.lower() in ["day", "rain", "et", "canopy", "biomass", "yield"]:
                        if key not in variables:
                            variables.append(key)
        
        print("\n" + "=" * 60)
        print("TIME SERIES")
        print("=" * 60)
        
        # Header
        header = "Day".ljust(6)
        for var in variables:
            header += f"{var[:12]:>12s}"
        print(header)
        print("-" * len(header))
        
        # Data rows
        for day in self.daily_data:
            row = str(int(day.get("Day", day.get("day", 0)))).ljust(6)
            for var in variables:
                val = day.get(var, 0)
                if isinstance(val, (int, float)):
                    row += f"{val:>12.2f}"
                else:
                    row += f"{str(val)[:12]:>12s}"
            print(row)
        
        print("=" * 60)
    
    def export_to_csv(self, output_file: Optional[Path] = None) -> Path:
        """Export results to CSV file.
        
        Args:
            output_file: Output file path. If None, uses default name.
        
        Returns:
            Path to the exported file.
        """
        if output_file is None:
            output_file = self.output_dir / f"{self.case_dir.name}_analysis.csv"
        
        if not self.daily_data:
            print("Error: No data to export")
            return output_file
        
        with open(output_file, "w", newline='') as f:
            writer = csv.writer(f)
            
            # Get headers
            headers = list(self.daily_data[0].keys())
            writer.writerow(headers)
            
            # Write data
            for day in self.daily_data:
                row = [day[h] for h in headers]
                writer.writerow(row)
        
        print(f"Exported results to: {output_file}")
        return output_file
    
    def export_to_json(self, output_file: Optional[Path] = None) -> Path:
        """Export results to JSON file.
        
        Args:
            output_file: Output file path. If None, uses default name.
        
        Returns:
            Path to the exported file.
        """
        if output_file is None:
            output_file = self.output_dir / f"{self.case_dir.name}_analysis.json"
        
        data = {
            "case": self.case_dir.name,
            "seasonal": self.seasonal_data,
            "daily": self.daily_data,
        }
        
        with open(output_file, "w") as f:
            json.dump(data, f, indent=2)
        
        print(f"Exported results to: {output_file}")
        return output_file
    
    def generate_report(self) -> str:
        """Generate a comprehensive analysis report.
        
        Returns:
            Report string.
        """
        if not self.daily_data:
            self.load_results()
        
        if not self.seasonal_data:
            self.calculate_seasonal_totals()
        
        report = []
        report.append("=" * 60)
        report.append(f"AQUACROP SIMULATION ANALYSIS REPORT")
        report.append(f"Case: {self.case_dir.name}")
        report.append("=" * 60)
        report.append("")
        
        # Seasonal summary
        report.append("1. SEASONAL SUMMARY")
        report.append("-" * 40)
        s = self.seasonal_data
        report.append(f"   Simulation period: {len(self.daily_data)} days")
        report.append(f"   Total rainfall: {s['total_rainfall']:.1f} mm")
        report.append(f"   Total irrigation: {s['total_irrigation']:.1f} mm")
        report.append(f"   Total ET: {s['total_et']:.1f} mm")
        report.append(f"   Final biomass: {s['max_biomass']:.1f} kg/ha")
        report.append(f"   Final yield: {s['final_yield']:.1f} kg/ha")
        report.append("")
        
        # Water use efficiency
        if s['total_et'] > 0:
            wue = s['final_yield'] / s['total_et']
            report.append("2. WATER USE EFFICIENCY")
            report.append("-" * 40)
            report.append(f"   WUE: {wue:.2f} kg/m³")
            report.append("")
        
        # Growth analysis
        report.append("3. GROWTH ANALYSIS")
        report.append("-" * 40)
        report.append(f"   Maximum canopy cover: {s['max_canopy']:.1f} %")
        report.append(f"   Average daily ET: {s['avg_et']:.2f} mm/day")
        report.append("")
        
        # Recommendations
        report.append("4. RECOMMENDATIONS")
        report.append("-" * 40)
        
        yield_tons = s['final_yield'] / 1000
        if yield_tons > 10:
            report.append("   ✓ Excellent yield (>10 t/ha)")
        elif yield_tons > 7:
            report.append("   ✓ Good yield (7-10 t/ha)")
        elif yield_tons > 5:
            report.append("   ~ Moderate yield (5-7 t/ha)")
        else:
            report.append("   ✗ Low yield (<5 t/ha) - consider:")
            report.append("     - Additional irrigation")
            report.append("     - Improved soil management")
            report.append("     - Adjusted planting date")
        
        report.append("")
        report.append("=" * 60)
        
        return "\n".join(report)


def main():
    """Main entry point."""
    import argparse
    
    parser = argparse.ArgumentParser(
        description="Analyze AquaCrop simulation results"
    )
    parser.add_argument(
        "case_dir",
        type=str,
        help="Path to case directory (e.g., PARAM/case-midwest-corn)"
    )
    parser.add_argument(
        "--export-csv",
        action="store_true",
        help="Export results to CSV file"
    )
    parser.add_argument(
        "--export-json",
        action="store_true",
        help="Export results to JSON file"
    )
    parser.add_argument(
        "--time-series",
        action="store_true",
        help="Print time series data"
    )
    parser.add_argument(
        "--stats",
        action="store_true",
        help="Print detailed statistics"
    )
    
    args = parser.parse_args()
    
    analyzer = ResultAnalyzer(Path(args.case_dir))
    
    if not analyzer.load_results():
        sys.exit(1)
    
    # Calculate seasonal totals
    analyzer.calculate_seasonal_totals()
    
    # Print seasonal summary
    analyzer.print_seasonal_summary()
    
    # Print daily statistics if requested
    if args.stats:
        analyzer.print_daily_statistics()
    
    # Print time series if requested
    if args.time_series:
        analyzer.print_time_series()
    
    # Export to CSV if requested
    if args.export_csv:
        analyzer.export_to_csv()
    
    # Export to JSON if requested
    if args.export_json:
        analyzer.export_to_json()
    
    # Generate comprehensive report
    print("\n" + analyzer.generate_report())


if __name__ == "__main__":
    main()
