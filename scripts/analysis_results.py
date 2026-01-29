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
    # Pattern to match: Day 1: biomass=115.0, canopy=4.5, transpiration=1.40, soil_moisture=25.7
    pattern = r"Day (\d+): biomass=([\d\.]+), canopy=([\d\.]+), transpiration=([\d\.]+), soil_moisture=([\d\.]+)"
    matches = re.findall(pattern, text)
    
    data = []
    for m in matches:
        data.append({
            'day': int(m[0]),
            'biomass': float(m[1]),
            'canopy': float(m[2]),
            'transpiration': float(m[3]),
            'soil_moisture': float(m[4])
        })
    return pd.DataFrame(data)

def main():
    # 1. Get C++ results
    cpp_output = run_cpp_model()
    if cpp_output is None:
        return
    
    # The C++ model runs both case-01 and case-02. We'll take the second one (case-02).
    # Since they are identical placeholders for now, we just split and take the second part or just parse all.
    # For this analysis, we'll just parse the whole thing and take the first 14 days.
    df_cpp = parse_results(cpp_output).iloc[:14]
    
    # 2. Get Classic/Reference results
    classic_file = 'PARAM/case-02/output/result.txt'
    if not os.path.exists(classic_file):
        print(f"Classic result file not found: {classic_file}")
        return
        
    with open(classic_file, 'r') as f:
        classic_content = f.read()
    df_classic = parse_results(classic_content)
    
    if df_cpp.empty or df_classic.empty:
        print("Could not parse results.")
        print(f"CPP rows: {len(df_cpp)}, Classic rows: {len(df_classic)}")
        return

    # 3. Visualization
    fig, axes = plt.subplots(2, 2, figsize=(15, 10))
    fig.suptitle('AquaCrop: C++ vs Classic Model Comparison (Placeholder Logic)', fontsize=16)

    metrics = [
        ('biomass', 'Biomass (kg/ha)', axes[0, 0]),
        ('canopy', 'Canopy Cover (%)', axes[0, 1]),
        ('transpiration', 'Transpiration (mm)', axes[1, 0]),
        ('soil_moisture', 'Soil Moisture (%)', axes[1, 1])
    ]

    for column, label, ax in metrics:
        ax.plot(df_classic['day'], df_classic[column], 'o-', label='Classic (Reference)', markersize=8, alpha=0.6)
        ax.plot(df_cpp['day'], df_cpp[column], 'x--', label='C++ Port', markersize=8)
        ax.set_xlabel('Day')
        ax.set_ylabel(label)
        ax.set_title(f'{label} over Time')
        ax.legend()
        ax.grid(True, linestyle='--', alpha=0.7)

    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    
    output_plot = 'docs/comparison_analysis.png'
    plt.savefig(output_plot)
    print(f"Analysis complete. Visualization saved to {output_plot}")
    
    # 4. Detailed Summary
    print("\n--- Summary Statistics ---")
    comparison = pd.DataFrame({
        'Metric': [m[0] for m in metrics],
        'Classic Mean': [df_classic[m[0]].mean() for m in metrics],
        'C++ Mean': [df_cpp[m[0]].mean() for m in metrics],
        'Max Absolute Error': [(df_cpp[m[0]] - df_classic[m[0]]).abs().max() for m in metrics]
    })
    print(comparison.to_string(index=False))

if __name__ == "__main__":
    main()
