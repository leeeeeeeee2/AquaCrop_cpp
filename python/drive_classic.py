#!/usr/bin/env python3
"""Python driver for the classic AquaCrop model.

This wrapper discovers test cases under:
- classic/AquaCrop/testcase (preferred)
and executes the classic model executable for each case, wiring
parameters from params.txt (or params/params.txt) when available.

Outputs are stored in each case's output/ subdirectory as:
- output/result.txt
- output/run.log
"""
from pathlib import Path
import argparse
import subprocess
import sys
import datetime
import json
from typing import Optional, List, Dict


def detect_executable(root: Path) -> Optional[Path]:
    exe = root / "classic" / "AquaCrop" / "aquacrop_main"
    if exe.exists() and exe.is_file():
        return exe
    exe = root / "classic" / "AquaCrop" / "aquacrop"
    if exe.exists() and exe.is_file():
        return exe
    # Fallback: try a previously-supposed mock location
    alt = root / "classic" / "AquaCrop" / "aquacrop_main"
    if alt.exists():
        return alt
    return None


def discover_cases(cases_root: Path):
    if not cases_root.exists():
        return []
    caselist = []
    for p in sorted(cases_root.iterdir()):
        if p.is_dir() and (p.name.startswith("case-") or p.name.startswith("Case-") or p.name.startswith("CASE-")):
            caselist.append(p)
    return caselist

def list_cases(cases_root: Path) -> List[Path]:
    """Return a list of case directories under cases_root."""
    return discover_cases(cases_root)


def load_params(case_dir: Path):
    # Try common locations for parameter files
    candidates = [case_dir / "params.txt", case_dir / "params" / "params.txt"]
    for p in candidates:
        if p.exists():
            content = p.read_text().strip()
            params = {}
            for line in content.splitlines():
                if "=" in line:
                    key, val = line.split("=", 1)
                    params[key.strip()] = val.strip()
            return params, p
    return {}, None

def extract_param_keys(param_path: Optional[Path]) -> List[str]:
    if param_path is None or not param_path.exists():
        return []
    try:
        text = param_path.read_text()
    except Exception:
        return []
    keys = []
    for line in text.splitlines():
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        if "=" in line:
            key = line.split("=", 1)[0].strip()
            if key:
                keys.append(key)
    return sorted(keys)


def run_case(case_dir: Path, exe_path: Path, param_path: Optional[Path], simulate: bool = False):
    output_dir = case_dir / "output"
    output_dir.mkdir(parents=True, exist_ok=True)
    out_path = output_dir / "result.txt"
    log_path = output_dir / "run.log"
    cmd = [str(exe_path)]
    if param_path is not None:
        cmd.append(str(param_path))
    if simulate:
        # Generate a deterministic simulation for 14 days as placeholder results
        with open(out_path, "w") as fout:
            fout.write("SIMULATED AquaCrop run (placeholder)\n")
            fout.write("Days: 14\n")
            fout.write("\n")
            fout.write("Day biomass(kg/ha) canopy(%) transpiration(mm) soil_moisture(%)\n")
            for day in range(1, 15):
                biomass = 100.0 + day * 15.0
                canopy = min(100.0, day * 4.5)
                transp = 0.8 + day * 0.6
                soil = 25.0 + day * 0.7
                fout.write(f"Day {day}: biomass={biomass:.1f}, canopy={canopy:.1f}, transpiration={transp:.2f}, soil_moisture={soil:.1f}\n")
        with open(log_path, "w") as ferr:
            ferr.write(f"SIMULATED run at {datetime.datetime.now().isoformat()}\n")
        ok = True
        return {
            "case_dir": str(case_dir),
            "success": ok,
            "param_path": str(param_path) if param_path else None,
            "output": str(out_path),
            "log": str(log_path),
        }
    else:
        with open(out_path, "w") as fout, open(log_path, "w") as ferr:
            try:
                ret = subprocess.run(cmd, stdout=fout, stderr=ferr, check=False)
                ok = (ret.returncode == 0)
            except Exception as e:
                ferr.write(f"Error during execution: {e}\n")
                ok = False
        return {
            "case_dir": str(case_dir),
            "success": ok,
            "param_path": str(param_path) if param_path else None,
            "output": str(out_path),
            "log": str(log_path),
        }


def main():
    parser = argparse.ArgumentParser(description="Drive classic AquaCrop model test cases.")
    parser.add_argument("root", nargs="?", default=None, help="Path to repo root (default: script location parent).")
    parser.add_argument("cases_path", nargs="?", default=None, help="Path to cases root (default: classic/AquaCrop/testcase).")
    parser.add_argument("--summary-out", dest="summary_out", help="Path to JSON summary output (optional).")
    parser.add_argument("--list-cases", dest="list_cases", action="store_true", help="List available test cases and exit.")
    parser.add_argument("--simulate", dest="simulate", action="store_true", help="Run simulated crop results instead of executing the binary.")
    parser.add_argument("--verbose", dest="verbose", action="store_true", help="Enable verbose debug output.")
    args = parser.parse_args()

    script_dir = Path(__file__).resolve()
    repo_root = Path(args.root).resolve() if args.root else script_dir.parent.parent
    cases_root = Path(args.cases_path).resolve() if args.cases_path else repo_root / "classic" / "AquaCrop" / "testcase"

    exe = detect_executable(repo_root)
    if exe is None:
        print("No executable found for classic AquaCrop. Aborting drive.")
        sys.exit(1)
    # Optional: list cases and exit
    if getattr(args, 'list_cases', False):
        for c in discover_cases(cases_root):
            print(str(c))
        sys.exit(0)

    cases = discover_cases(cases_root)
    if not cases:
        print(f"No test cases found under {cases_root}. Creating a default sample case.")
        sample_case = cases_root / "case-01"
        sample_case.mkdir(parents=True, exist_ok=True)
        sample_param = sample_case / "params.txt"
        if not sample_param.exists():
            sample_param.write_text("param1=default_case01\nparam2=default\n")
        cases = [sample_case]

    results: List[Dict] = []
    for case in cases:
        params, param_path = load_params(case)
        res = run_case(case, exe, param_path, simulate=args.simulate)
        results.append(res)

    # Human-friendly summary
    print("Drive Summary:")
    for r in results:
        keys = extract_param_keys(Path(r["param_path"]) if r["param_path"] else None)
        print(f"- {Path(r['case_dir']).name}: {'OK' if r['success'] else 'FAIL'}; Params={keys}; Output={Path(r['output']).name}")

    # Optional JSON summary
    if args.summary_out:
        Path(args.summary_out).parent.mkdir(parents=True, exist_ok=True)
        Path(args.summary_out).write_text(json.dumps(results, indent=2))

    print("Done driving classic AquaCrop cases.")


if __name__ == "__main__":
    main()
