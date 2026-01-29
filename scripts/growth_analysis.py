import subprocess
import re
import matplotlib.pyplot as plt
import pandas as pd
import os

def run_cpp_model():
    print("Running C++ model...")
    result = subprocess.run(['./build/aquacrop_main'], capture_output=True, text=True)
    if result.returncode != 0:
        print("Error running C++ model:")
        print(result.stderr)
        return None
    return result.stdout

def parse_results(text):
    # Updated pattern to match: Project: case-01/project.ACp Day 1: biomass=115.0, canopy=4.5, ...
    pattern = r"Project: ([\w\-/.]+) Day (\d+): biomass=([\d\.]+), canopy=([\d\.]+), transpiration=([\d\.]+), soil_moisture=([\d\.]+)"
    matches = re.findall(pattern, text)
    
    data = []
    for m in matches:
        data.append({
            'project': m[0],
            'day': int(m[1]),
            'biomass': float(m[2]),
            'canopy': float(m[3]),
            'transpiration': float(m[4]),
            'soil_moisture': float(m[5])
        })
    return pd.DataFrame(data)

def main():
    # 1. Get C++ results
    cpp_output = run_cpp_model()
    if cpp_output is None:
        return
    
    df = parse_results(cpp_output)
    if df.empty:
        print("No data parsed from C++ model output.")
        return

    projects = df['project'].unique()
    print(f"Detected projects: {projects}")

    # 2. Visualization
    fig, axes = plt.subplots(2, 2, figsize=(16, 12))
    fig.suptitle('AquaCrop C++ Model: Growth Analysis Comparison', fontsize=20)

    metrics = [
        ('biomass', 'Biomass (kg/ha)', axes[0, 0]),
        ('canopy', 'Canopy Cover (%)', axes[0, 1]),
        ('transpiration', 'Transpiration (mm)', axes[1, 0]),
        ('soil_moisture', 'Soil Moisture (%)', axes[1, 1])
    ]

    colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728']
    
    for i, project in enumerate(projects):
        df_p = df[df['project'] == project]
        linestyle = '-' if i == 0 else '--'
        marker = 'o' if i == 0 else 's'
        
        for j, (column, label, ax) in enumerate(metrics):
            ax.plot(df_p['day'], df_p[column], marker=marker, linestyle=linestyle, 
                    label=f'Project: {project}', color=colors[i % len(colors)])
            ax.set_xlabel('Day After Planting (DAP)')
            ax.set_ylabel(label)
            ax.set_title(f'{label} Progression')
            ax.legend()
            ax.grid(True, linestyle=':', alpha=0.6)

    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    output_plot = 'docs/growth_analysis_comparison.png'
    plt.savefig(output_plot)
    print(f"Analysis complete. Visualization saved to {output_plot}")
    
    # 3. Growth Analysis Summary
    print("\n" + "="*50)
    print("      CROP GROWTH ANALYSIS SUMMARY")
    print("="*50)
    
    for project in projects:
        df_p = df[df['project'] == project]
        max_biomass = df_p['biomass'].max()
        max_canopy = df_p['canopy'].max()
        avg_transp = df_p['transpiration'].mean()
        
        print(f"\nProject: {project}")
        print(f"  - Max Biomass Reached: {max_biomass:.1f} kg/ha")
        print(f"  - Max Canopy Cover:    {max_canopy:.1f}%")
        print(f"  - Avg Transpiration:   {avg_transp:.2f} mm/day")
        
        # Calculate growth rate
        growth_rate = (df_p['biomass'].iloc[-1] - df_p['biomass'].iloc[0]) / (df_p['day'].iloc[-1] - df_p['day'].iloc[0])
        print(f"  - Avg Biomass Growth:  {growth_rate:.2f} kg/ha/day")

    if len(projects) >= 2:
        p1, p2 = projects[0], projects[1]
        df1, df2 = df[df['project'] == p1], df[df['project'] == p2]
        biomass_diff = (df2['biomass'].max() / df1['biomass'].max() - 1) * 100
        print(f"\nComparative Analysis:")
        print(f"  - {p2} shows {biomass_diff:+.1f}% biomass difference compared to {p1}")

if __name__ == "__main__":
    main()
