# AquaCrop C++ Visualization Tools

This directory contains Python scripts for visualizing AquaCrop C++ simulation results.

## Overview

Comprehensive visualization tools have been created to analyze and display AquaCrop simulation results using matplotlib and seaborn. These tools work with both placeholder simulation results (current C++ implementation) and will be ready for real results once core algorithms are implemented.

## Available Scripts

### 1. `visualize_results.py`

Main visualization script for generating plots and charts.

#### Features

- **Dashboard Visualization**: Comprehensive overview with 8 panels showing all key variables
- **Growth Analysis**: Detailed crop growth curves with growth rates and phases
- **Water Balance**: Water use efficiency, cumulative transpiration, and soil moisture dynamics
- **Case Comparison**: Side-by-side comparison of multiple simulation cases

#### Usage

```bash
# Generate dashboard for single case
python visualize_results.py PARAM/case-01 --type dashboard --output visualization/case-01_dashboard.png

# Generate growth analysis
python visualize_results.py PARAM/case-01 --type growth --output visualization/case-01_growth.png

# Generate water balance analysis
python visualize_results.py PARAM/case-01 --type water --output visualization/case-01_water.png

# Compare multiple cases
python visualize_results.py PARAM/case-01 PARAM/case-02 --type compare --output visualization/comparison.png

# Adjust DPI for high-quality output
python visualize_results.py PARAM/case-01 --type dashboard --output visualization/case-01_dashboard.png --dpi 600
```

#### Visualization Types

1. **Dashboard** (default)
   - Biomass accumulation curve
   - Final biomass value display
   - Canopy development
   - Daily transpiration
   - Soil moisture dynamics
   - Cumulative water use
   - Water use efficiency
   - Statistics summary

2. **Growth**
   - Biomass accumulation with growth rate
   - Canopy development with maximum tracking
   - Daily transpiration with totals
   - Soil moisture with zone shading (wilting point, available water, field capacity)

3. **Water Balance**
   - Cumulative water use
   - Water use efficiency over time
   - Soil moisture vs transpiration scatter plot
   - Growth phase analysis

4. **Compare**
   - Biomass comparison across cases
   - Canopy comparison across cases
   - Transpiration comparison across cases
   - Soil moisture comparison across cases

### 2. `generate_report.py`

Generate interactive HTML reports with embedded visualizations.

#### Features

- Professional HTML layout with CSS styling
- Embedded visualizations
- Statistics summary tables
- Case comparison tables
- Sectioned content with navigation
- Responsive design

#### Usage

```bash
# Generate report with default settings
python generate_report.py

# Specify output file and title
python generate_report.py --output visualization/report.html --title "My Simulation Report"
```

#### Report Structure

1. **Introduction**: Overview of AquaCrop and current implementation status
2. **Simulation Results Summary**: Description of test cases
3. **Case Statistics**: Individual case summaries with key metrics
4. **Comparison Table**: Side-by-side comparison of all cases
5. **Visualizations**: Embedded images of all plots
6. **Analysis and Conclusions**: Observations and next steps

## Generated Visualizations

### Case-01 Dashboard
- **File**: `visualization/case-01_dashboard.png`
- **Size**: ~567 KB
- **Content**: 8-panel dashboard with comprehensive analysis

### Case-01 Growth Analysis
- **File**: `visualization/case-01_growth.png`
- **Size**: ~407 KB
- **Content**: 4-panel growth analysis

### Case-01 Water Balance
- **File**: `visualization/case-01_water.png`
- **Size**: ~453 KB
- **Content**: 4-panel water balance analysis

### Case-02 Dashboard
- **File**: `visualization/case-02_dashboard.png`
- **Size**: ~568 KB
- **Content**: 8-panel dashboard

### Case Comparison
- **File**: `visualization/comparison.png`
- **Size**: Generated on-demand
- **Content**: 4-panel comparison

### HTML Report
- **File**: `visualization/simulation_report.html`
- **Content**: Interactive HTML with all visualizations

## Requirements

### Python Dependencies

```bash
pip install matplotlib seaborn numpy pandas
```

- **matplotlib**: 3.10.8+
- **seaborn**: 0.13.2+
- **numpy**: 2.4.1+
- **pandas**: 3.0.0+

### System Requirements

- Python 3.8+
- 4GB RAM minimum
- Disk space for output images (1-2 MB per visualization)

## Output Format

### Image Files

- **Format**: PNG
- **Default DPI**: 300 (high quality)
- **Dimensions**: 14×10 inches for most plots
- **Color**: Professional color palette (Seaborn husl)

### HTML Report

- **Format**: HTML5
- **Styling**: Embedded CSS
- **Responsiveness**: Mobile-friendly
- **Browser**: Any modern browser

## Visualization Details

### Dashboard Panels

1. **Biomass Accumulation** (2 columns wide)
   - Time series with fill
   - Growth rate annotation
   - Color: Teal (#1f77b4)

2. **Final Biomass**
   - Large value display
   - Units: kg/ha
   - Color: Teal

3. **Canopy Development**
   - 0-100% y-axis
   - Maximum canopy annotation
   - Fill shading
   - Color: Orange (#ff7f0e)

4. **Daily Transpiration**
   - Daily values with fill
   - Total and average statistics
   - Color: Green (#2ca02c)

5. **Soil Moisture**
   - 0-100% y-axis
   - Zone shading:
     - Red (0-15%): Wilting Point
     - Green (15-35%): Available Water
     - Blue (35-100%): Field Capacity
   - Color: Red (#d62728)

6. **Cumulative Transpiration**
   - Cumulative sum over time
   - Fill shading
   - Color: Purple (#9467bd)

7. **Water Use Efficiency**
   - Biomass / Cumulative Transpiration
   - Final value annotation
   - Color: Brown (#8c564b)

8. **Statistics Summary**
   - Text-based summary panel
   - Key metrics and totals

### Growth Analysis Panels

1. **Biomass Accumulation**
   - Time series with fill
   - Growth rate annotation in text box

2. **Canopy Development**
   - Time series with fill
   - Maximum canopy line
   - Value annotation at peak

3. **Daily Transpiration**
   - Time series with fill
   - Total and average in text box

4. **Soil Moisture Dynamics**
   - Time series without fill
   - Three zone shading bands
   - Legend for moisture zones

### Water Balance Panels

1. **Cumulative Water Use**
   - Linear accumulation
   - Fill shading

2. **Water Use Efficiency**
   - Calculated as Biomass / Cumulative Transpiration
   - Final value line
   - Value annotation

3. **Soil Moisture vs Transpiration**
   - Scatter plot with color gradient
   - Days as color variable (viridis colormap)
   - Colorbar included

4. **Growth Phases**
   - Scatter plot of growth rates
   - Color-coded by growth phase:
     - Green: Rapid growth
     - Orange: Moderate growth
     - Red: Slow growth
   - Legend included

### Comparison Panels

For each variable (biomass, canopy, transpiration, soil moisture):

- Multiple series on same axes
- Different colors for each case
- Legend showing case names
- Consistent styling across panels

## Customization

### Color Palette

Change colors by modifying the color palette:

```python
# In visualize_results.py
self.colors = sns.color_palette("husl", 8)  # Current
self.colors = sns.color_palette("viridis", 8)  # Alternative
self.colors = ['#ff0000', '#00ff00', '#0000ff', ...]  # Custom
```

### Figure Size

Adjust in the `__init__` method:

```python
def __init__(self, figsize: Tuple[int, int] = (14, 10)):
    self.figsize = figsize
```

Or pass custom size when creating visualizer:

```python
visualizer = AquaCropVisualizer(figsize=(12, 8))
```

### DPI

For publication-quality or web images:

```bash
# High resolution for printing
python visualize_results.py PARAM/case-01 --dpi 600

# Lower resolution for web
python visualize_results.py PARAM/case-01 --dpi 150
```

## Extending the Tools

### Adding New Visualization Types

1. Add new method to `AquaCropVisualizer` class
2. Follow existing pattern:
   ```python
   def plot_new_type(self, data: SimulationData, output_file: Optional[Path] = None) -> None:
       # Create figure and axes
       fig, axes = plt.subplots(...)
       
       # Plot data
       axes[0, 0].plot(...)
       
       # Add styling
       axes[0, 0].set_xlabel(...)
       axes[0, 0].grid(True)
       
       # Save or show
       if output_file:
           plt.savefig(output_file, dpi=300)
       plt.show()
   ```
3. Add to command-line argument choices
4. Update documentation

### Adding New Data Variables

1. Add to `SimulationData` class:
   ```python
   def __init__(self, case_name: str):
       self.new_variable = []
   ```
2. Update parsing logic
3. Update visualization methods to use new variable

## Troubleshooting

### Import Errors

If you see "No module named 'matplotlib'":

```bash
pip install matplotlib seaborn
```

### Empty Plots

If plots are empty or show no data:
- Check that `result.txt` file exists in case/output/
- Verify file contains simulation data
- Check for parsing errors in console output

### Type Errors

If you see type errors:
- Ensure Python 3.8+ is being used
- Check that numpy arrays are created correctly
- Verify data is numeric, not string

### Memory Issues

For large simulations:
- Reduce DPI: `--dpi 150`
- Process fewer variables at a time
- Increase system RAM

## Examples

### Quick Visualization

```bash
# Generate all visualizations for case-01
python visualize_results.py PARAM/case-01 --type dashboard
python visualize_results.py PARAM/case-01 --type growth
python visualize_results.py PARAM/case-01 --type water
```

### Batch Processing

```bash
# Generate dashboards for all cases
for case in PARAM/case-*; do
    python visualize_results.py "$case" --type dashboard \
        --output "visualization/${case##*/}_dashboard.png"
done
```

### Complete Workflow

```bash
# 1. Run simulation
./build/bin/aquacrop_main

# 2. Generate all visualizations
python visualize_results.py PARAM/case-01 --type dashboard --output viz/case-01_dash.png
python visualize_results.py PARAM/case-01 --type growth --output viz/case-01_growth.png
python visualize_results.py PARAM/case-01 --type water --output viz/case-01_water.png

# 3. Generate HTML report
python generate_report.py --output viz/report.html

# 4. Open report
open viz/report.html  # macOS
xdg-open viz/report.html  # Linux
start viz/report.html  # Windows
```

## Performance

- **Dashboard generation**: ~2-3 seconds
- **Growth analysis**: ~1-2 seconds
- **Water balance**: ~2 seconds
- **Comparison**: ~2-3 seconds
- **HTML report**: <1 second

## Future Enhancements

Planned improvements:

1. **Interactive Plots**: Plotly for interactive web visualizations
2. **Animation**: Time-lapse animations of crop growth
3. **3D Visualization**: 3D plots of soil moisture profiles
4. **Geospatial**: Map-based visualizations for regional studies
5. **Real-time**: Live plotting during simulation
6. **PDF Export**: Multi-page PDF reports
7. **Video**: Generate video from daily animations

## References

- **Matplotlib Documentation**: https://matplotlib.org/stable/contents.html
- **Seaborn Documentation**: https://seaborn.pydata.org/
- **FAO AquaCrop**: https://www.fao.org/aquacrop/

## License

Same as AquaCrop C++ project (MIT License).

## Support

For issues or questions:
1. Check this README
2. Review error messages carefully
3. Verify dependencies are installed
4. Check data file format

---

**Version**: 1.0
**Last Updated**: January 29, 2026
**Author**: OpenCode AI Assistant
