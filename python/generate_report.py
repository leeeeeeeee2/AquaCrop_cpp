#!/usr/bin/env python3
"""
Generate HTML report with embedded visualizations.

This script creates an interactive HTML report containing
all AquaCrop simulation results and visualizations.
"""

import sys
from pathlib import Path
from typing import List, Optional
import json


class HTMLReportGenerator:
    """Generate HTML reports for AquaCrop simulations."""
    
    def __init__(self, output_file: str = "simulation_report.html"):
        """Initialize report generator.
        
        Args:
            output_file: Name of output HTML file.
        """
        self.output_file = Path(output_file)
        self.content = []
        self.sections = []
        
    def add_header(self, title: str, subtitle: str = "") -> None:
        """Add report header.
        
        Args:
            title: Report title.
            subtitle: Optional subtitle.
        """
        self.content.append(f"""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{title}</title>
    <style>
        body {{
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 0;
            padding: 20px;
            background-color: #f5f5f5;
            color: #333;
        }}
        .container {{
            max-width: 1400px;
            margin: 0 auto;
            background-color: white;
            padding: 30px;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
            border-radius: 8px;
        }}
        .header {{
            text-align: center;
            margin-bottom: 40px;
            border-bottom: 3px solid #4CAF50;
            padding-bottom: 20px;
        }}
        .header h1 {{
            color: #2E7D32;
            margin: 0;
            font-size: 2.5em;
        }}
        .header p {{
            color: #666;
            font-size: 1.2em;
            margin: 10px 0 0 0;
        }}
        .section {{
            margin: 30px 0;
            padding: 20px;
            background-color: #f9f9f9;
            border-left: 4px solid #4CAF50;
            border-radius: 4px;
        }}
        .section h2 {{
            color: #2E7D32;
            margin-top: 0;
            font-size: 1.8em;
        }}
        .case-card {{
            background-color: white;
            border: 1px solid #ddd;
            border-radius: 8px;
            padding: 20px;
            margin: 20px 0;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }}
        .case-card h3 {{
            color: #388E3C;
            margin-top: 0;
            font-size: 1.5em;
        }}
        .stats-grid {{
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
            gap: 15px;
            margin: 20px 0;
        }}
        .stat-box {{
            background-color: #E8F5E9;
            padding: 15px;
            border-radius: 8px;
            text-align: center;
        }}
        .stat-value {{
            font-size: 2em;
            font-weight: bold;
            color: #2E7D32;
        }}
        .stat-label {{
            color: #666;
            font-size: 0.9em;
            margin-top: 5px;
        }}
        .visualization {{
            margin: 30px 0;
            text-align: center;
        }}
        .visualization img {{
            max-width: 100%;
            height: auto;
            border: 1px solid #ddd;
            border-radius: 4px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }}
        .caption {{
            color: #666;
            font-style: italic;
            margin: 10px 0 0 0;
        }}
        table {{
            width: 100%;
            border-collapse: collapse;
            margin: 20px 0;
        }}
        th, td {{
            padding: 12px;
            text-align: left;
            border-bottom: 1px solid #ddd;
        }}
        th {{
            background-color: #4CAF50;
            color: white;
            font-weight: bold;
        }}
        tr:hover {{
            background-color: #f5f5f5;
        }}
        .note {{
            background-color: #FFF3E0;
            border-left: 4px solid #FF9800;
            padding: 15px;
            margin: 20px 0;
            border-radius: 4px;
        }}
        .footer {{
            text-align: center;
            margin-top: 50px;
            padding: 20px;
            background-color: #f5f5f5;
            border-top: 2px solid #4CAF50;
            color: #666;
        }}
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>{title}</h1>
            {f'<p>{subtitle}</p>' if subtitle else ''}
        </div>
""")
    
    def add_section(self, title: str, content: str) -> None:
        """Add a section to the report.
        
        Args:
            title: Section title.
            content: Section content (HTML).
        """
        self.content.append(f"""
        <div class="section">
            <h2>{title}</h2>
            {content}
        </div>
""")
    
    def add_case_summary(self, case_name: str, stats: dict) -> None:
        """Add a case summary card.
        
        Args:
            case_name: Name of the case.
            stats: Dictionary with statistics.
        """
        html_content = f"""
        <div class="case-card">
            <h3>{case_name}</h3>
            <div class="stats-grid">
                <div class="stat-box">
                    <div class="stat-value">{stats.get('simulation_days', 0)}</div>
                    <div class="stat-label">Simulation Days</div>
                </div>
                <div class="stat-box">
                    <div class="stat-value">{stats.get('final_biomass', 0):.1f}</div>
                    <div class="stat-label">Final Biomass (kg/ha)</div>
                </div>
                <div class="stat-box">
                    <div class="stat-value">{stats.get('max_canopy', 0):.1f}%</div>
                    <div class="stat-label">Max Canopy Cover</div>
                </div>
                <div class="stat-box">
                    <div class="stat-value">{stats.get('total_transpiration', 0):.1f}</div>
                    <div class="stat-label">Total Transpiration (mm)</div>
                </div>
                <div class="stat-box">
                    <div class="stat-value">{stats.get('avg_transpiration', 0):.2f}</div>
                    <div class="stat-label">Avg Transpiration (mm/day)</div>
                </div>
                <div class="stat-box">
                    <div class="stat-value">{stats.get('growth_rate', 0):.2f}</div>
                    <div class="stat-label">Biomass Growth Rate (kg/ha/day)</div>
                </div>
            </div>
        </div>
"""
        self.content.append(html_content)
    
    def add_visualization(self, title: str, image_path: Path, caption: str = "") -> None:
        """Add a visualization to the report.
        
        Args:
            title: Visualization title.
            image_path: Path to image file.
            caption: Optional caption.
        """
        if not image_path.exists():
            print(f"Warning: Image not found: {image_path}")
            return
        
        html_content = f"""
        <div class="visualization">
            <h3>{title}</h3>
            <img src="{image_path}" alt="{title}">
            {f'<p class="caption">{caption}</p>' if caption else ''}
        </div>
"""
        self.content.append(html_content)
    
    def add_comparison_table(self, cases: List[dict]) -> None:
        """Add a comparison table for multiple cases.
        
        Args:
            cases: List of dictionaries with case data.
        """
        html_content = """
        <table>
            <tr>
                <th>Case</th>
                <th>Days</th>
                <th>Final Biomass (kg/ha)</th>
                <th>Max Canopy (%)</th>
                <th>Total Transp. (mm)</th>
                <th>Avg Transp. (mm/day)</th>
                <th>Growth Rate (kg/ha/day)</th>
            </tr>
"""
        for case in cases:
            html_content += f"""
            <tr>
                <td><strong>{case.get('name', 'N/A')}</strong></td>
                <td>{case.get('days', 0)}</td>
                <td>{case.get('biomass', 0):.1f}</td>
                <td>{case.get('canopy', 0):.1f}</td>
                <td>{case.get('total_transp', 0):.1f}</td>
                <td>{case.get('avg_transp', 0):.2f}</td>
                <td>{case.get('growth_rate', 0):.2f}</td>
            </tr>
"""
        html_content += """
        </table>
"""
        self.content.append(html_content)
    
    def add_note(self, content: str) -> None:
        """Add a note box.
        
        Args:
            content: Note content.
        """
        html_content = f"""
        <div class="note">
            <strong>Note:</strong> {content}
        </div>
"""
        self.content.append(html_content)
    
    def add_footer(self) -> None:
        """Add report footer."""
        from datetime import datetime
        
        self.content.append(f"""
        <div class="footer">
            <p>Report generated on {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}</p>
            <p>AquaCrop C++ Simulation Results Visualization</p>
        </div>
    </div>
</body>
</html>
""")
    
    def generate(self) -> Path:
        """Generate the HTML report.
        
        Returns:
            Path to generated file.
        """
        # Ensure output directory exists
        self.output_file.parent.mkdir(parents=True, exist_ok=True)
        
        # Write content to file
        with open(self.output_file, 'w', encoding='utf-8') as f:
            f.writelines(self.content)
        
        print(f"Generated HTML report: {self.output_file}")
        return self.output_file


def main():
    """Main entry point."""
    import argparse
    
    parser = argparse.ArgumentParser(
        description="Generate HTML report for AquaCrop simulations"
    )
    parser.add_argument(
        "--output",
        type=str,
        default="visualization/simulation_report.html",
        help="Output HTML file (default: visualization/simulation_report.html)"
    )
    parser.add_argument(
        "--title",
        type=str,
        default="AquaCrop C++ Simulation Results",
        help="Report title"
    )
    
    args = parser.parse_args()
    
    # Create report
    report = HTMLReportGenerator(args.output)
    
    # Add header
    report.add_header(args.title, "Comprehensive Analysis of Crop Simulation Results")
    
    # Add introduction
    report.add_section(
        "Introduction",
        """
        <p>This report presents the results of AquaCrop C++ simulations. The AquaCrop model
        is a crop-water productivity model developed by FAO that simulates yield response
        of herbaceous crops to water availability.</p>
        
        <p><strong>Current Status:</strong> The C++ implementation is in <em>placeholder mode</em>,
        meaning results are generated with simulated formulas rather than full AquaCrop algorithms.
        The infrastructure is complete, but core simulation algorithms (soil water balance,
        crop growth models, stress factors) are still being implemented.</p>
        """
    )
    
    # Add case summaries
    report.add_section(
        "Simulation Results Summary",
        """
        <p>The following test cases were simulated:</p>
        <ul>
            <li><strong>case-01:</strong> 14-day placeholder simulation</li>
            <li><strong>case-02:</strong> 14-day placeholder simulation</li>
            <li><strong>case-midwest-corn:</strong> U.S. Midwest corn test case (pending results)</li>
        </ul>
        """
    )
    
    # Add case statistics
    case01_stats = {
        'simulation_days': 14,
        'final_biomass': 310.0,
        'max_canopy': 63.0,
        'total_transpiration': 74.2,
        'avg_transpiration': 5.30,
        'growth_rate': 13.93
    }
    report.add_case_summary("case-01", case01_stats)
    
    case02_stats = {
        'simulation_days': 14,
        'final_biomass': 310.0,
        'max_canopy': 63.0,
        'total_transpiration': 74.2,
        'avg_transpiration': 5.30,
        'growth_rate': 13.93
    }
    report.add_case_summary("case-02", case02_stats)
    
    # Add comparison table
    cases = [
        {
            'name': 'case-01',
            'days': 14,
            'biomass': 310.0,
            'canopy': 63.0,
            'total_transp': 74.2,
            'avg_transp': 5.30,
            'growth_rate': 13.93
        },
        {
            'name': 'case-02',
            'days': 14,
            'biomass': 310.0,
            'canopy': 63.0,
            'total_transp': 74.2,
            'avg_transp': 5.30,
            'growth_rate': 13.93
        }
    ]
    report.add_comparison_table(cases)
    
    # Add visualizations
    report.add_section(
        "Visualizations",
        "<p>The following visualizations provide detailed analysis of simulation results.</p>"
    )
    
    # Add dashboard visualizations
    report.add_visualization(
        "Case-01 Dashboard",
        Path("visualization/case-01_dashboard.png"),
        "Comprehensive dashboard showing biomass accumulation, canopy development, transpiration, and soil moisture dynamics for case-01."
    )
    
    report.add_visualization(
        "Case-02 Dashboard",
        Path("visualization/case-02_dashboard.png"),
        "Comprehensive dashboard showing biomass accumulation, canopy development, transpiration, and soil moisture dynamics for case-02."
    )
    
    report.add_visualization(
        "Case-01 Growth Analysis",
        Path("visualization/case-01_growth.png"),
        "Detailed crop growth curves showing biomass accumulation, canopy development, transpiration, and soil moisture."
    )
    
    report.add_visualization(
        "Case-01 Water Balance",
        Path("visualization/case-01_water.png"),
        "Water balance analysis including cumulative water use, water use efficiency, and growth phases."
    )
    
    report.add_visualization(
        "Case Comparison",
        Path("visualization/comparison.png"),
        "Side-by-side comparison of case-01 and case-02 results."
    )
    
    # Add important note
    report.add_note(
        "These results are from placeholder simulations. The AquaCrop C++ implementation "
        "is still under development. Once the core algorithms are implemented, "
        "results will reflect realistic crop growth and soil water balance calculations "
        "based on FAO AquaCrop methodology."
    )
    
    # Add analysis and conclusions
    report.add_section(
        "Analysis and Conclusions",
        """
        <h3>Key Observations</h3>
        <ul>
            <li><strong>Infrastructure Ready:</strong> The build system, file I/O, and project parsing 
            infrastructure are fully operational.</li>
            <li><strong>Test Case Generation:</strong> Midwest corn test case with realistic parameters 
            has been successfully created.</li>
            <li><strong>Visualization Tools:</strong> Comprehensive visualization scripts are available 
            for analyzing results.</li>
            <li><strong>Placeholder Results:</strong> Current results use simulated formulas rather than 
            full AquaCrop algorithms.</li>
        </ul>
        
        <h3>Next Steps</h3>
        <ul>
            <li>Implement soil water balance module in C++</li>
            <li>Add crop growth models</li>
            <li>Include stress factor calculations</li>
            <li>Validate against FAO AquaCrop reference</li>
            <li>Test with real-world data</li>
        </ul>
        
        <h3>Technical Details</h3>
        <p><strong>Build System:</strong> CMake with C++17 standard</p>
        <p><strong>Visualization:</strong> Python with matplotlib and seaborn</p>
        <p><strong>Data Format:</strong> Placeholder format with daily biomass, canopy, transpiration, and soil moisture</p>
        """
    )
    
    # Add footer and generate
    report.add_footer()
    report.generate()
    
    print(f"\nOpen the report in your browser: file://{report.output_file.resolve()}")


if __name__ == "__main__":
    main()
