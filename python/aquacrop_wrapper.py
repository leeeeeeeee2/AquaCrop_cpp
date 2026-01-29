"""A lightweight Python wrapper to build and run the C++ AquaCrop executable.

Usage:
    runner = AquaCropRunner(repo_root="/path/to/AquaCrop_cpp")
    result = runner.run_project("case-01/project.ACp")
    print(result.stdout)
"""

import os
import shutil
import subprocess
from dataclasses import dataclass
from typing import Optional

@dataclass
class RunResult:
    returncode: int
    stdout: str
    stderr: str

class AquaCropRunner:
    def __init__(self, repo_root: Optional[str] = None):
        self.repo_root = os.path.abspath(repo_root or os.path.join(os.path.dirname(__file__), ".."))
        self.build_dir = os.path.join(self.repo_root, "build")
        self.executable = os.path.join(self.build_dir, "aquacrop_main")
        self.list_projects_path = os.path.join(self.repo_root, "PARAM", "ListProjects.txt")

    def build(self) -> None:
        """Configure and build the project using CMake."""
        if not os.path.exists(self.build_dir):
            os.makedirs(self.build_dir)
        # Configure
        subprocess.run(["cmake", "-S", self.repo_root, "-B", self.build_dir], check=True)
        # Build
        subprocess.run(["cmake", "--build", self.build_dir, "--config", "Release"], check=True)

    def _read_list_projects(self) -> str:
        if os.path.exists(self.list_projects_path):
            with open(self.list_projects_path, "r", encoding="utf-8") as f:
                return f.read()
        return ""

    def _write_list_projects(self, content: str) -> None:
        with open(self.list_projects_path, "w", encoding="utf-8") as f:
            f.write(content)

    def run_project(self, project_path: str, build_if_missing: bool = True) -> RunResult:
        """Run a single project. `project_path` is relative to `PARAM/` (for example: 'case-01/project.ACp').

        This method temporarily modifies `PARAM/ListProjects.txt` to contain only `project_path` and restores the previous file contents afterwards.
        """
        original_content = self._read_list_projects()
        try:
            # Ensure executable exists (build if requested)
            if build_if_missing and not os.path.isfile(self.executable):
                self.build()

            # Overwrite list projects file temporarily
            temp_content = project_path.strip() + "\n"
            self._write_list_projects(temp_content)

            # Run the executable from the repo root so relative paths like PARAM/ work
            proc = subprocess.run([self.executable], cwd=self.repo_root, capture_output=True, text=True)
            return RunResult(proc.returncode, proc.stdout, proc.stderr)
        finally:
            # Restore original list
            self._write_list_projects(original_content)

if __name__ == "__main__":
    runner = AquaCropRunner()
    print("Building (if needed) and running 'case-01/project.ACp'...")
    res = runner.run_project("case-01/project.ACp")
    print("--- stdout ---")
    print(res.stdout)
    print("--- stderr ---")
    print(res.stderr)
    print("return code:", res.returncode)
