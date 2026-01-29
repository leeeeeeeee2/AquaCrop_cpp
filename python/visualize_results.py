#!/usr/bin/env python3
"""
Visualization tools for AquaCrop C++ simulation results.

This script creates comprehensive visualizations of crop growth,
water balance, and simulation results using matplotlib and seaborn.
"""

import sys
import re
from pathlib import Path
from typing import Dict, List, Optional, Tuple
import numpy as np

try:
    import matplotlib.pyplot as plt
    import matplotlib.dates as mdates
    import seaborn as sns
    from matplotlib.gridspec import GridSpec

    # Set style
    sns.set_style("whitegrid")
    sns.set_palette("husl")
except ImportError as e:
    print(f"Error: Required visualization libraries not installed.")
    print(f"Please install: pip install matplotlib seaborn")
    sys.exit(1)


class SimulationData:
    """Container for simulation data."""
    
    def __init__(self, case_name: str):
        """Initialize simulation data container.
        
        Args:
            case_name: Name of the simulation case.
        """
        self.case_name = case_name
        self.days = []
        self.biomass = []
        self.canopy = []
        self.transpiration = []
        self.soil_moisture = []
        self.rainfall = []
        self.irrigation = []
        self.et = []
        self.crop_yield = []
        
    def load_from_placeholder(self, result_file: Path) -> bool:
        """Load data from placeholder result file.
        
        Args:
            result_file: Path to result.txt file.
        
        Returns:
            True if loading succeeded, False otherwise.
        """
        if not result_file.exists():
            print(f"Error: Result file not found: {result_file}")
            return False
        
        with open(result_file, "r") as f:
            content = f.read()
            
            if "SIMULATED AquaCrop run (placeholder)" not in content:
                print("Warning: Not a placeholder result file")
                return False
            
            # Parse data
            pattern = r"Day (\d+): biomass=([\d.]+), canopy=([\d.]+), transpiration=([\d.]+), soil_moisture=([\d.]+)"
            matches = re.findall(pattern, content)
            
            for match in matches:
                self.days.append(int(match[0]))
                self.biomass.append(float(match[1]))
                self.canopy.append(float(match[2]))
                self.transpiration.append(float(match[3]))
                self.soil_moisture.append(float(match[4]))
            
            # Placeholder data for rainfall, irrigation, ET
            self.rainfall = [0.0] * len(self.days)
            self.irrigation = [0.0] * len(self.days)
            self.et = self.transpiration.copy()
            
            print(f"Loaded {len(self.days)} days of data for {self.case_name}")
            return len(self.days) > 0
    
    def get_numpy_arrays(self) -> Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray]:
        """Convert data to numpy arrays.
        
        Returns:
            Tuple of (days, biomass, canopy, transpiration, soil_moisture)
        """
        return (
            np.array(self.days),
            np.array(self.biomass),
            np.array(self.canopy),
            np.array(self.transpiration),
            np.array(self.soil_moisture)
        )


class AquaCropVisualizer:
    """Visualize AquaCrop simulation results."""
    
    def __init__(self, figsize: Tuple[int, int] = (14, 10)):
        """Initialize visualizer.
        
        Args:
            figsize: Figure size (width, height) in inches.
        """
        self.figsize = figsize
        self.colors = sns.color_palette("husl", 8)
        
    def plot_crop_growth(self, data: SimulationData, output_file: Optional[Path] = None) -> None:
        """Plot crop growth curves.
        
        Args:
            data: Simulation data.
            output_file: Optional output file path.
        """
        days, biomass, canopy, transp, sm = data.get_numpy_arrays()
        
        fig, axes = plt.subplots(2, 2, figsize=self.figsize)
        fig.suptitle(f"Crop Growth Simulation - {data.case_name}", fontsize=16, fontweight='bold')
        
        # Biomass accumulation
        ax1 = axes[0, 0]
        ax1.plot(days, biomass, 'o-', linewidth=2, markersize=6, color=self.colors[0])
        ax1.fill_between(days, 0, biomass, alpha=0.3, color=self.colors[0])
        ax1.set_xlabel('Day', fontsize=12)
        ax1.set_ylabel('Biomass (kg/ha)', fontsize=12)
        ax1.set_title('Biomass Accumulation', fontsize=13, fontweight='bold')
        ax1.grid(True, alpha=0.3)
        
        # Calculate and display growth rate
        if len(days) > 1:
            growth_rate = (biomass[-1] - biomass[0]) / len(days)
            ax1.text(0.05, 0.95, f'Growth Rate: {growth_rate:.2f} kg/ha/day',
                    transform=ax1.transAxes, fontsize=10,
                    verticalalignment='top', bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5))
        
        # Canopy development
        ax2 = axes[0, 1]
        ax2.plot(days, canopy, 's-', linewidth=2, markersize=6, color=self.colors[1])
        ax2.fill_between(days, 0, canopy, alpha=0.3, color=self.colors[1])
        ax2.set_xlabel('Day', fontsize=12)
        ax2.set_ylabel('Canopy Cover (%)', fontsize=12)
        ax2.set_title('Canopy Development', fontsize=13, fontweight='bold')
        ax2.grid(True, alpha=0.3)
        ax2.set_ylim(0, 100)
        
        # Display max canopy
        max_canopy = np.max(canopy)
        max_day = days[np.argmax(canopy)]
        ax2.axhline(y=max_canopy, color='red', linestyle='--', alpha=0.7, linewidth=1.5)
        ax2.text(max_day, max_canopy, f' Max: {max_canopy:.1f}%',
                fontsize=9, verticalalignment='bottom')
        
        # Transpiration
        ax3 = axes[1, 0]
        ax3.plot(days, transp, '^-', linewidth=2, markersize=6, color=self.colors[2])
        ax3.fill_between(days, 0, transp, alpha=0.3, color=self.colors[2])
        ax3.set_xlabel('Day', fontsize=12)
        ax3.set_ylabel('Transpiration (mm)', fontsize=12)
        ax3.set_title('Daily Transpiration', fontsize=13, fontweight='bold')
        ax3.grid(True, alpha=0.3)
        
        # Display total transpiration
        total_transp = np.sum(transp)
        avg_transp = np.mean(transp)
        ax3.text(0.05, 0.95, f'Total: {total_transp:.1f} mm\nAvg: {avg_transp:.2f} mm/day',
                transform=ax3.transAxes, fontsize=10,
                verticalalignment='top', bbox=dict(boxstyle='round', facecolor='lightblue', alpha=0.5))
        
        # Soil moisture
        ax4 = axes[1, 1]
        ax4.plot(days, sm, 'D-', linewidth=2, markersize=6, color=self.colors[3])
        ax4.set_xlabel('Day', fontsize=12)
        ax4.set_ylabel('Soil Moisture (%)', fontsize=12)
        ax4.set_title('Soil Moisture Dynamics', fontsize=13, fontweight='bold')
        ax4.grid(True, alpha=0.3)
        
        # Add shading for moisture zones
        ax4.axhspan(0, 15, color='red', alpha=0.1, label='Wilting Point')
        ax4.axhspan(15, 35, color='green', alpha=0.1, label='Available Water')
        ax4.axhspan(35, 100, color='blue', alpha=0.1, label='Field Capacity')
        ax4.legend(loc='upper right', fontsize=9)
        
        plt.tight_layout()
        
        if output_file:
            plt.savefig(output_file, dpi=300, bbox_inches='tight')
            print(f"Saved crop growth plot: {output_file}")
        
        plt.show()
    
    def plot_water_balance(self, data: SimulationData, output_file: Optional[Path] = None) -> None:
        """Plot water balance components.
        
        Args:
            data: Simulation data.
            output_file: Optional output file path.
        """
        days, biomass, canopy, transp, sm = data.get_numpy_arrays()
        
        fig, axes = plt.subplots(2, 2, figsize=self.figsize)
        fig.suptitle(f"Water Balance - {data.case_name}", fontsize=16, fontweight='bold')
        
        # Cumulative water use
        ax1 = axes[0, 0]
        cum_transp = np.cumsum(transp)
        ax1.plot(days, cum_transp, 'o-', linewidth=2, markersize=6, color=self.colors[4])
        ax1.fill_between(days, 0, cum_transp, alpha=0.3, color=self.colors[4])
        ax1.set_xlabel('Day', fontsize=12)
        ax1.set_ylabel('Cumulative Transpiration (mm)', fontsize=12)
        ax1.set_title('Cumulative Water Use', fontsize=13, fontweight='bold')
        ax1.grid(True, alpha=0.3)
        
        # Water use efficiency
        ax2 = axes[0, 1]
        wue = biomass / cum_transp
        ax2.plot(days, wue, 's-', linewidth=2, markersize=6, color=self.colors[5])
        ax2.set_xlabel('Day', fontsize=12)
        ax2.set_ylabel('Water Use Efficiency (kg/m³)', fontsize=12)
        ax2.set_title('Water Use Efficiency', fontsize=13, fontweight='bold')
        ax2.grid(True, alpha=0.3)
        
        # Final WUE
        final_wue = wue[-1]
        ax2.axhline(y=final_wue, color='red', linestyle='--', alpha=0.7, linewidth=1.5)
        ax2.text(days[-1], final_wue, f' {final_wue:.2f} kg/m³',
                fontsize=9, verticalalignment='bottom')
        
        # Soil moisture vs transpiration
        ax3 = axes[1, 0]
        scatter = ax3.scatter(sm, transp, c=days, cmap='viridis', s=80, alpha=0.6, edgecolors='black')
        ax3.set_xlabel('Soil Moisture (%)', fontsize=12)
        ax3.set_ylabel('Transpiration (mm/day)', fontsize=12)
        ax3.set_title('Soil Moisture vs Transpiration', fontsize=13, fontweight='bold')
        ax3.grid(True, alpha=0.3)
        plt.colorbar(scatter, ax=ax3, label='Day')
        
        # Growth phase indicators
        ax4 = axes[1, 1]
        
        # Calculate growth phases
        growth_rates = np.diff(biomass)
        phases = []
        for i, rate in enumerate(growth_rates):
            if rate > np.median(growth_rates):
                phases.append('Rapid')
            elif rate > np.median(growth_rates) * 0.5:
                phases.append('Moderate')
            else:
                phases.append('Slow')
        
        phase_days = days[1:]
        phase_colors = {'Rapid': 'green', 'Moderate': 'orange', 'Slow': 'red'}
        phase_colors_list = [phase_colors.get(p, 'gray') for p in phases]
        
        ax4.scatter(phase_days, growth_rates, c=phase_colors_list, s=80, alpha=0.6, edgecolors='black')
        ax4.set_xlabel('Day', fontsize=12)
        ax4.set_ylabel('Biomass Growth Rate (kg/ha/day)', fontsize=13, fontweight='bold')
        ax4.set_title('Growth Phases', fontsize=13, fontweight='bold')
        ax4.grid(True, alpha=0.3)
        ax4.axhline(y=0, color='black', linestyle='-', linewidth=0.5)
        
        # Add legend
        from matplotlib.lines import Line2D
        legend_elements = [Line2D([0], [0], marker='o', color='w', label='Rapid',
                          markerfacecolor='green', markersize=10),
                         Line2D([0], [0], marker='o', color='w', label='Moderate',
                          markerfacecolor='orange', markersize=10),
                         Line2D([0], [0], marker='o', color='w', label='Slow',
                          markerfacecolor='red', markersize=10)]
        ax4.legend(handles=legend_elements, loc='upper right', fontsize=9)
        
        plt.tight_layout()
        
        if output_file:
            plt.savefig(output_file, dpi=300, bbox_inches='tight')
            print(f"Saved water balance plot: {output_file}")
        
        plt.show()
    
    def plot_summary_dashboard(self, data: SimulationData, output_file: Optional[Path] = None) -> None:
        """Create a comprehensive summary dashboard.
        
        Args:
            data: Simulation data.
            output_file: Optional output file path.
        """
        days, biomass, canopy, transp, sm = data.get_numpy_arrays()
        
        fig = plt.figure(figsize=(16, 12))
        gs = GridSpec(3, 3, figure=fig, hspace=0.3, wspace=0.3)
        
        fig.suptitle(f"AquaCrop Simulation Dashboard - {data.case_name}",
                    fontsize=18, fontweight='bold')
        
        # Main growth curve (spanning 2 columns)
        ax1 = fig.add_subplot(gs[0, :2])
        ax1.plot(days, biomass, 'o-', linewidth=2.5, markersize=7, color=self.colors[0])
        ax1.fill_between(days, 0, biomass, alpha=0.3, color=self.colors[0])
        ax1.set_xlabel('Day', fontsize=12)
        ax1.set_ylabel('Biomass (kg/ha)', fontsize=12)
        ax1.set_title('Biomass Accumulation', fontsize=14, fontweight='bold')
        ax1.grid(True, alpha=0.3)
        
        # Add growth rate indicator
        if len(days) > 1:
            growth_rate = (biomass[-1] - biomass[0]) / len(days)
            ax1.text(0.02, 0.98, f'Growth Rate: {growth_rate:.2f} kg/ha/day',
                    transform=ax1.transAxes, fontsize=11,
                    verticalalignment='top',
                    bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.7))
        
        # Final biomass
        ax2 = fig.add_subplot(gs[0, 2])
        final_biomass = biomass[-1]
        ax2.text(0.5, 0.7, f'{final_biomass:.0f}', transform=ax2.transAxes,
                fontsize=36, ha='center', fontweight='bold', color=self.colors[0])
        ax2.text(0.5, 0.3, 'kg/ha', transform=ax2.transAxes,
                fontsize=14, ha='center')
        ax2.set_title('Final Biomass', fontsize=14, fontweight='bold')
        ax2.axis('off')
        
        # Canopy development
        ax3 = fig.add_subplot(gs[1, 0])
        ax3.plot(days, canopy, 's-', linewidth=2, markersize=6, color=self.colors[1])
        ax3.fill_between(days, 0, canopy, alpha=0.3, color=self.colors[1])
        ax3.set_xlabel('Day', fontsize=11)
        ax3.set_ylabel('Canopy (%)', fontsize=11)
        ax3.set_title('Canopy Cover', fontsize=13, fontweight='bold')
        ax3.grid(True, alpha=0.3)
        ax3.set_ylim(0, 100)
        
        # Transpiration
        ax4 = fig.add_subplot(gs[1, 1])
        ax4.plot(days, transp, '^-', linewidth=2, markersize=6, color=self.colors[2])
        ax4.fill_between(days, 0, transp, alpha=0.3, color=self.colors[2])
        ax4.set_xlabel('Day', fontsize=11)
        ax4.set_ylabel('Transpiration (mm)', fontsize=11)
        ax4.set_title('Daily Transpiration', fontsize=13, fontweight='bold')
        ax4.grid(True, alpha=0.3)
        
        # Soil moisture
        ax5 = fig.add_subplot(gs[1, 2])
        ax5.plot(days, sm, 'D-', linewidth=2, markersize=6, color=self.colors[3])
        ax5.set_xlabel('Day', fontsize=11)
        ax5.set_ylabel('Soil Moisture (%)', fontsize=11)
        ax5.set_title('Soil Moisture', fontsize=13, fontweight='bold')
        ax5.grid(True, alpha=0.3)
        
        # Cumulative water use
        ax6 = fig.add_subplot(gs[2, 0])
        cum_transp = np.cumsum(transp)
        ax6.plot(days, cum_transp, 'o-', linewidth=2, markersize=6, color=self.colors[4])
        ax6.fill_between(days, 0, cum_transp, alpha=0.3, color=self.colors[4])
        ax6.set_xlabel('Day', fontsize=11)
        ax6.set_ylabel('Cum. Transpiration (mm)', fontsize=11)
        ax6.set_title('Cumulative Water Use', fontsize=13, fontweight='bold')
        ax6.grid(True, alpha=0.3)
        
        # Water use efficiency
        ax7 = fig.add_subplot(gs[2, 1])
        wue = biomass / (cum_transp + 1e-6)
        ax7.plot(days, wue, 's-', linewidth=2, markersize=6, color=self.colors[5])
        ax7.set_xlabel('Day', fontsize=11)
        ax7.set_ylabel('WUE (kg/m³)', fontsize=11)
        ax7.set_title('Water Use Efficiency', fontsize=13, fontweight='bold')
        ax7.grid(True, alpha=0.3)
        
        # Statistics summary
        ax8 = fig.add_subplot(gs[2, 2])
        ax8.axis('off')
        
        stats_text = f"""
        SIMULATION SUMMARY
        {'='*25}
        
        Days: {len(days)}
        
        BIOMASS
        Initial:  {biomass[0]:.1f} kg/ha
        Final:    {biomass[-1]:.1f} kg/ha
        Max:      {np.max(biomass):.1f} kg/ha
        
        CANOPY
        Max:      {np.max(canopy):.1f}%
        Avg:      {np.mean(canopy):.1f}%
        
        WATER
        Total ET:   {np.sum(transp):.1f} mm
        Avg ET:     {np.mean(transp):.2f} mm/day
        Final WUE:  {wue[-1]:.2f} kg/m³
        """
        ax8.text(0.05, 0.95, stats_text, transform=ax8.transAxes,
                fontsize=10, verticalalignment='top', family='monospace')
        
        plt.tight_layout()
        
        if output_file:
            plt.savefig(output_file, dpi=300, bbox_inches='tight')
            print(f"Saved dashboard plot: {output_file}")
        
        plt.show()
    
    def compare_cases(self, data_list: List[SimulationData], output_file: Optional[Path] = None) -> None:
        """Compare multiple simulation cases.
        
        Args:
            data_list: List of simulation data objects.
            output_file: Optional output file path.
        """
        fig, axes = plt.subplots(2, 2, figsize=self.figsize)
        fig.suptitle('Multi-Case Comparison', fontsize=16, fontweight='bold')
        
        for i, data in enumerate(data_list):
            days, biomass, canopy, transp, sm = data.get_numpy_arrays()
            color = self.colors[i % len(self.colors)]
            
            # Biomass comparison
            axes[0, 0].plot(days, biomass, 'o-', linewidth=2, markersize=6,
                            label=data.case_name, color=color)
            
            # Canopy comparison
            axes[0, 1].plot(days, canopy, 's-', linewidth=2, markersize=6,
                            label=data.case_name, color=color)
            
            # Transpiration comparison
            axes[1, 0].plot(days, transp, '^-', linewidth=2, markersize=6,
                            label=data.case_name, color=color)
            
            # Soil moisture comparison
            axes[1, 1].plot(days, sm, 'D-', linewidth=2, markersize=6,
                            label=data.case_name, color=color)
        
        # Set labels and titles
        axes[0, 0].set_xlabel('Day')
        axes[0, 0].set_ylabel('Biomass (kg/ha)')
        axes[0, 0].set_title('Biomass Accumulation', fontweight='bold')
        axes[0, 0].legend(fontsize=9)
        axes[0, 0].grid(True, alpha=0.3)
        
        axes[0, 1].set_xlabel('Day')
        axes[0, 1].set_ylabel('Canopy Cover (%)')
        axes[0, 1].set_title('Canopy Development', fontweight='bold')
        axes[0, 1].legend(fontsize=9)
        axes[0, 1].grid(True, alpha=0.3)
        axes[0, 1].set_ylim(0, 100)
        
        axes[1, 0].set_xlabel('Day')
        axes[1, 0].set_ylabel('Transpiration (mm)')
        axes[1, 0].set_title('Daily Transpiration', fontweight='bold')
        axes[1, 0].legend(fontsize=9)
        axes[1, 0].grid(True, alpha=0.3)
        
        axes[1, 1].set_xlabel('Day')
        axes[1, 1].set_ylabel('Soil Moisture (%)')
        axes[1, 1].set_title('Soil Moisture Dynamics', fontweight='bold')
        axes[1, 1].legend(fontsize=9)
        axes[1, 1].grid(True, alpha=0.3)
        
        plt.tight_layout()
        
        if output_file:
            plt.savefig(output_file, dpi=300, bbox_inches='tight')
            print(f"Saved comparison plot: {output_file}")
        
        plt.show()


def load_multiple_cases(case_dirs: List[Path]) -> List[SimulationData]:
    """Load multiple simulation cases.
    
    Args:
        case_dirs: List of case directory paths.
    
    Returns:
        List of SimulationData objects.
    """
    data_list = []
    
    for case_dir in case_dirs:
        case_name = case_dir.name
        result_file = case_dir / "output" / "result.txt"
        
        if not result_file.exists():
            print(f"Warning: Result file not found for {case_name}")
            continue
        
        data = SimulationData(case_name)
        if data.load_from_placeholder(result_file):
            data_list.append(data)
    
    return data_list


def main():
    """Main entry point."""
    import argparse
    
    parser = argparse.ArgumentParser(
        description="Visualize AquaCrop C++ simulation results"
    )
    parser.add_argument(
        "case_dir",
        nargs='+',
        type=str,
        help="Path to case directory(ies)"
    )
    parser.add_argument(
        "--type",
        type=str,
        default="dashboard",
        choices=["dashboard", "growth", "water", "compare"],
        help="Visualization type (default: dashboard)"
    )
    parser.add_argument(
        "--output",
        type=str,
        help="Output file path (optional)"
    )
    parser.add_argument(
        "--dpi",
        type=int,
        default=300,
        help="DPI for output images (default: 300)"
    )
    
    args = parser.parse_args()
    
    # Load data
    case_dirs = [Path(d) for d in args.case_dir]
    data_list = load_multiple_cases(case_dirs)
    
    if not data_list:
        print("Error: No data loaded")
        sys.exit(1)
    
    # Create visualizer
    visualizer = AquaCropVisualizer()
    
    # Generate visualizations
    output_file = Path(args.output) if args.output else None
    
    if args.type == "compare" and len(data_list) > 1:
        visualizer.compare_cases(data_list, output_file)
    elif args.type == "growth":
        for data in data_list:
            if len(data_list) > 1:
                output = None if not output_file else f"{output_file.stem}_{data.case_name}{output_file.suffix}"
            else:
                output = output_file
            visualizer.plot_crop_growth(data, output)
    elif args.type == "water":
        for data in data_list:
            if len(data_list) > 1:
                output = None if not output_file else f"{output_file.stem}_{data.case_name}{output_file.suffix}"
            else:
                output = output_file
            visualizer.plot_water_balance(data, output)
    else:  # dashboard
        for data in data_list:
            if len(data_list) > 1:
                output = None if not output_file else f"{output_file.stem}_{data.case_name}{output_file.suffix}"
            else:
                output = output_file
            visualizer.plot_summary_dashboard(data, output)


if __name__ == "__main__":
    main()
