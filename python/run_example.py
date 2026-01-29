"""Example: build and run case-01 with the Python wrapper."""
from aquacrop_wrapper import AquaCropRunner

if __name__ == "__main__":
    runner = AquaCropRunner()
    result = runner.run_project("case-01/project.ACp")
    print("Return code:", result.returncode)
    print(result.stdout)
    if result.stderr:
        print("Errors:\n", result.stderr)
