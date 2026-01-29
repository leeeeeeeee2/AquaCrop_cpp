#!/usr/bin/env python3
"""
Improved Python driver for AquaCrop C++ model.

This driver provides enhanced functionality for:
- Managing multiple test cases
- Running simulations with various configurations
- Collecting and analyzing results
- Supporting batch processing and parameter studies
"""

from pathlib import Path
import argparse
import subprocess
import sys
import datetime
import json
import csv
from typing import Optional, List, Dict, Tuple, Any
import shutil


class AquaCropDriver:
    """Enhanced driver for AquaCrop C++ simulations."""
    
    def __init__(self, root_dir: Optional[Path] = None, exe_path: Optional[Path] = None):
        """Initialize the driver.
        
        Args:
            root_dir: Root directory of the project. If None, uses current directory.
            exe_path: Path to aquacrop executable. If None, auto-detects.
        """
        self.root_dir = Path(root_dir).resolve() if root_dir else Path(__file__).parent.parent
        self.exe_path = exe_path or self._detect_executable()
        self.params_dir = self.root_dir / "PARAM"
        self.list_file = self.root_dir / "LIST" / "ListProjects.txt"
        
    def _detect_executable(self) -> Optional[Path]:
        """Detect the AquaCrop executable."""
        candidates = [
            self.root_dir / "build" / "aquacrop_main",
            self.root_dir / "build" / "aquacrop",
            self.root_dir / "aquacrop_main",
            self.root_dir / "aquacrop",
        ]
        for exe in candidates:
            if exe.exists() and exe.is_file():
                return exe
        return None
    
    def list_cases(self) -> List[Path]:
        """List all available test cases."""
        if not self.params_dir.exists():
            return []
        cases = []
        for p in sorted(self.params_dir.iterdir()):
            if p.is_dir() and (p.name.startswith("case-") or 
                              p.name.startswith("Case-") or 
                              p.name.startswith("CASE-")):
                cases.append(p)
        return cases
    
    def build_model(self) -> bool:
        """Build the AquaCrop model.
        
        Returns:
            True if build succeeded, False otherwise.
        """
        build_dir = self.root_dir / "build"
        
        if not build_dir.exists():
            build_dir.mkdir(parents=True, exist_ok=True)
        
        try:
            print(f"Building AquaCrop in {build_dir}...")
            subprocess.run(
                ["cmake", ".."],
                cwd=build_dir,
                check=True,
                capture_output=True
            )
            subprocess.run(
                ["cmake", "--build", ".", "-j4"],
                cwd=build_dir,
                check=True,
                capture_output=True
            )
            print("Build completed successfully!")
            return True
        except subprocess.CalledProcessError as e:
            print(f"Build failed: {e}")
            print(f"STDOUT: {e.stdout.decode() if e.stdout else 'N/A'}")
            print(f"STDERR: {e.stderr.decode() if e.stderr else 'N/A'}")
            return False
    
    def create_case(
        self,
        case_name: str,
        description: str,
        crop_type: str = "Maize",
        planting_date: str = "2024-05-01",
        harvest_date: str = "2024-10-15"
    ) -> Path:
        """Create a new test case directory structure.
        
        Args:
            case_name: Name of the case (e.g., "case-midwest").
            description: Description of the case.
            crop_type: Type of crop.
            planting_date: Planting date (YYYY-MM-DD).
            harvest_date: Harvest date (YYYY-MM-DD).
        
        Returns:
            Path to the created case directory.
        """
        case_dir = self.params_dir / case_name
        case_dir.mkdir(parents=True, exist_ok=True)
        
        # Create subdirectories
        (case_dir / "CLIM").mkdir(exist_ok=True)
        (case_dir / "CROP").mkdir(exist_ok=True)
        (case_dir / "SOIL").mkdir(exist_ok=True)
        (case_dir / "IRRIG").mkdir(exist_ok=True)
        (case_dir / "MANAGE").mkdir(exist_ok=True)
        (case_dir / "GWT").mkdir(exist_ok=True)
        (case_dir / "OFF").mkdir(exist_ok=True)
        (case_dir / "OBS").mkdir(exist_ok=True)
        (case_dir / "output").mkdir(exist_ok=True)
        (case_dir / "params").mkdir(exist_ok=True)
        
        # Create params.txt
        params_file = case_dir / "params.txt"
        params_content = f"""# Parameters for {case_name}
crop_type={crop_type}
planting_date={planting_date}
harvest_date={harvest_date}
description={description}
created={datetime.datetime.now().isoformat()}
"""
        params_file.write_text(params_content)
        
        print(f"Created test case: {case_dir}")
        return case_dir
    
    def generate_project_file(
        self,
        case_dir: Path,
        num_years: int = 1,
        start_year: int = 2024
    ) -> Path:
        """Generate a project.ACp file for the case.
        
        Args:
            case_dir: Path to the case directory.
            num_years: Number of simulation years.
            start_year: Starting year.
        
        Returns:
            Path to the created project file.
        """
        project_file = case_dir / "project.ACp"
        case_name = case_dir.name
        
        # Parse planting and harvest dates
        params = self._load_params(case_dir)
        plant_date = params.get("planting_date", "2024-05-01")
        harvest_date = params.get("harvest_date", "2024-10-15")
        
        plant_month, plant_day = map(int, plant_date.split("-")[1:])
        harvest_month, harvest_day = map(int, harvest_date.split("-")[1:])
        
        content = f"""{case_name} - AquaCrop C++ Project
7.2
1
1
{num_years}
1
{num_years}
1
{num_years}
Climate data for {case_name}
climate.TXT
CLIM/
Temperature data for {case_name}
temperature.TXT
CLIM/
Reference evapotranspiration for {case_name}
eto.TXT
CLIM/
Rainfall data for {case_name}
rain.TXT
CLIM/
CO2 concentration for {case_name}
co2.TXT
CLIM/
Calendar data for {case_name}
calendar.TXT
CLIM/
Crop parameters for {case_name}
crop.TXT
CROP/
Irrigation schedule for {case_name}
irrigation.TXT
IRRIG/
Management practices for {case_name}
management.TXT
MANAGE/
Soil profile for {case_name}
soil.TXT
SOIL/
Groundwater data for {case_name}
groundwater.TXT
GWT/
Initial soil water content for {case_name}
swcini.TXT
SOIL/
Off-season data for {case_name}
offseason.TXT
OFF/
Field observations for {case_name}
observations.TXT
OBS/
"""
        project_file.write_text(content)
        print(f"Generated project file: {project_file}")
        return project_file
    
    def _load_params(self, case_dir: Path) -> Dict[str, str]:
        """Load parameters from params.txt."""
        params_file = case_dir / "params.txt"
        if not params_file.exists():
            return {}
        
        params = {}
        with open(params_file) as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith("#"):
                    continue
                if "=" in line:
                    key, val = line.split("=", 1)
                    params[key.strip()] = val.strip()
        return params
    
    def register_case(self, case_dir: Path) -> None:
        """Register a case in the list file.
        
        Args:
            case_dir: Path to the case directory.
        """
        if not self.list_file.parent.exists():
            self.list_file.parent.mkdir(parents=True, exist_ok=True)
        
        project_path = case_dir / "project.ACp"
        if not project_path.exists():
            raise FileNotFoundError(f"Project file not found: {project_path}")
        
        # Append to list file
        with open(self.list_file, "a") as f:
            f.write(f"{case_dir.name}/project.ACp\n")
        
        print(f"Registered case: {case_dir.name}")
    
    def run_case(
        self,
        case_dir: Path,
        verbose: bool = False
    ) -> Dict[str, Any]:
        """Run a single test case.
        
        Args:
            case_dir: Path to the case directory.
            verbose: Enable verbose output.
        
        Returns:
            Dictionary with run results.
        """
        if self.exe_path is None:
            raise RuntimeError("AquaCrop executable not found. Run build_model() first.")
        
        output_dir = case_dir / "output"
        output_dir.mkdir(parents=True, exist_ok=True)
        
        result_file = output_dir / "result.txt"
        log_file = output_dir / "run.log"
        
        print(f"Running case: {case_dir.name}")
        
        with open(result_file, "w") as fout, open(log_file, "w") as ferr:
            try:
                result = subprocess.run(
                    [str(self.exe_path)],
                    cwd=self.root_dir,
                    stdout=fout,
                    stderr=ferr,
                    check=False
                )
                success = result.returncode == 0
            except Exception as e:
                ferr.write(f"Error during execution: {e}\n")
                success = False
        
        if verbose:
            with open(log_file, "r") as f:
                print(f.read())
        
        return {
            "case": case_dir.name,
            "success": success,
            "output": str(result_file),
            "log": str(log_file),
            "returncode": result.returncode if 'result' in locals() else -1,
        }
    
    def run_all_cases(self, verbose: bool = False) -> List[Dict[str, Any]]:
        """Run all registered test cases.
        
        Args:
            verbose: Enable verbose output.
        
        Returns:
            List of run results for each case.
        """
        cases = self.list_cases()
        results = []
        
        print(f"Found {len(cases)} test cases")
        
        for case in cases:
            result = self.run_case(case, verbose=verbose)
            results.append(result)
        
        return results
    
    def parse_results(self, case_dir: Path) -> Optional[Dict[str, Any]]:
        """Parse simulation results from output directory.
        
        Args:
            case_dir: Path to the case directory.
        
        Returns:
            Dictionary with parsed results, or None if parsing failed.
        """
        output_dir = case_dir / "output"
        result_file = output_dir / "result.txt"
        
        if not result_file.exists():
            return None
        
        results = {
            "case": case_dir.name,
            "daily": [],
            "seasonal": {},
        }
        
        try:
            with open(result_file, "r") as f:
                content = f.read()
                
                # Try to parse as CSV
                lines = [line.strip() for line in content.splitlines() if line.strip()]
                if len(lines) > 1:
                    reader = csv.DictReader(lines[1:], fieldnames=lines[0].split(","))
                    for row in reader:
                        results["daily"].append(row)
                    
                    # Calculate seasonal totals
                    if results["daily"]:
                        last_day = results["daily"][-1]
                        results["seasonal"] = {
                            "final_biomass": last_day.get("biomass", 0),
                            "final_yield": last_day.get("yield", 0),
                            "total_days": len(results["daily"]),
                        }
            
            return results
            
        except Exception as e:
            print(f"Error parsing results: {e}")
            return None
    
    def summarize_results(self, results: List[Dict[str, Any]]) -> str:
        """Generate a summary of run results.
        
        Args:
            results: List of run results.
        
        Returns:
            Formatted summary string.
        """
        lines = [
            "=" * 60,
            "AquaCrop Simulation Summary",
            "=" * 60,
            f"Total cases: {len(results)}",
            f"Successful: {sum(1 for r in results if r['success'])}",
            f"Failed: {sum(1 for r in results if not r['success'])}",
            "",
            "Case Results:",
        ]
        
        for r in results:
            status = "✓ OK" if r["success"] else "✗ FAIL"
            lines.append(f"  {r['case']}: {status}")
        
        return "\n".join(lines)


def main():
    """Main entry point."""
    parser = argparse.ArgumentParser(
        description="Improved Python driver for AquaCrop C++",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Build the model
  python improved_driver.py build
  
  # List available cases
  python improved_driver.py list
  
  # Run all cases
  python improved_driver.py run
  
  # Run specific case
  python improved_driver.py run --case case-01
  
  # Create new case
  python improved_driver.py create --name case-midwest --crop Maize
        """
    )
    
    subparsers = parser.add_subparsers(dest="command", help="Available commands")
    
    # Build command
    build_parser = subparsers.add_parser("build", help="Build the AquaCrop model")
    
    # List command
    list_parser = subparsers.add_parser("list", help="List available test cases")
    
    # Create command
    create_parser = subparsers.add_parser("create", help="Create a new test case")
    create_parser.add_argument("--name", required=True, help="Case name (e.g., case-midwest)")
    create_parser.add_argument("--description", default="Custom test case", help="Case description")
    create_parser.add_argument("--crop", default="Maize", help="Crop type (e.g., Maize, Wheat, Soybean)")
    create_parser.add_argument("--planting", default="2024-05-01", help="Planting date (YYYY-MM-DD)")
    create_parser.add_argument("--harvest", default="2024-10-15", help="Harvest date (YYYY-MM-DD)")
    
    # Run command
    run_parser = subparsers.add_parser("run", help="Run simulation(s)")
    run_parser.add_argument("--case", help="Specific case to run (default: all)")
    run_parser.add_argument("--verbose", "-v", action="store_true", help="Verbose output")
    
    # Parse command
    parse_parser = subparsers.add_parser("parse", help="Parse simulation results")
    parse_parser.add_argument("--case", required=True, help="Case to parse")
    
    args = parser.parse_args()
    
    if not args.command:
        parser.print_help()
        sys.exit(1)
    
    driver = AquaCropDriver()
    
    if args.command == "build":
        success = driver.build_model()
        sys.exit(0 if success else 1)
    
    elif args.command == "list":
        cases = driver.list_cases()
        print(f"Available test cases ({len(cases)}):")
        for case in cases:
            print(f"  - {case.name}")
        sys.exit(0)
    
    elif args.command == "create":
        case_dir = driver.create_case(
            args.name,
            args.description,
            args.crop,
            args.planting,
            args.harvest
        )
        driver.generate_project_file(case_dir)
        print(f"\nCase created: {case_dir}")
        print(f"To register this case, add '{args.name}/project.ACp' to LIST/ListProjects.txt")
        sys.exit(0)
    
    elif args.command == "run":
        if args.case:
            case_dir = driver.params_dir / args.case
            if not case_dir.exists():
                print(f"Error: Case not found: {case_dir}")
                sys.exit(1)
            results = [driver.run_case(case_dir, verbose=args.verbose)]
        else:
            results = driver.run_all_cases(verbose=args.verbose)
        
        print(driver.summarize_results(results))
        sys.exit(0 if all(r["success"] for r in results) else 1)
    
    elif args.command == "parse":
        case_dir = driver.params_dir / args.case
        results = driver.parse_results(case_dir)
        if results:
            print(json.dumps(results, indent=2))
            sys.exit(0)
        else:
            print("Error: Could not parse results")
            sys.exit(1)


if __name__ == "__main__":
    main()
