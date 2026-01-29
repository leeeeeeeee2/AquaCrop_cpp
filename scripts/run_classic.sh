#!/usr/bin/env bash
set -euo pipefail

# Run the classic AquaCrop model (If present) and store results for test comparison.
# Tries to build the classic sources under `classic/AquaCrop` and locate an executable.
# Test results and parameters are written to either /test (if writable) or ./test.

ROOT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
CLASSIC_DIR="$ROOT_DIR/classic/AquaCrop"
  TARGET_TEST_ROOT="$ROOT_DIR/test"  # Local test directory to store results

echo "[Info] AquaCrop classic runner starting."
echo "[Info] Root: $ROOT_DIR"

if [ ! -d "$CLASSIC_DIR" ]; then
  echo "[Warn] Classic AquaCrop directory not found at $CLASSIC_DIR. Skipping run."
  exit 0
fi

## Prepare test directories (local) for results; avoid using /test in this environment
shopt -s nullglob
CASE_DIRS_FIRST=("$TARGET_TEST_ROOT"/case-*/)
if [ ${#CASE_DIRS_FIRST[@]} -eq 0 ]; then
  echo "[Info] Creating default test cases: case-01 and case-02"
  mkdir -p "$TARGET_TEST_ROOT/case-01/params" "$TARGET_TEST_ROOT/case-01/input" "$TARGET_TEST_ROOT/case-01/output"
  mkdir -p "$TARGET_TEST_ROOT/case-02/params" "$TARGET_TEST_ROOT/case-02/input" "$TARGET_TEST_ROOT/case-02/output"
  cat > "$TARGET_TEST_ROOT/case-01/params/params.txt" << 'EOF'
param1=default_case01
param2=default
EOF
  cat > "$TARGET_TEST_ROOT/case-02/params/params.txt" << 'EOF'
param1=default_case02
param2=alternate
EOF
fi

# Build if Makefile exists
if [ -f "$CLASSIC_DIR/Makefile" ] || [ -f "$CLASSIC_DIR/makefile" ]; then
  echo "[Info] Building classic AquaCrop..."
  (cd "$CLASSIC_DIR" && make) || echo "[Warn] Build step failed or not possible; continuing to search for executable."
fi

# Locate executable
CLASSIC_BIN=""
if [ -x "$CLASSIC_DIR/aquacrop_main" ]; then CLASSIC_BIN="$CLASSIC_DIR/aquacrop_main"; fi
if [ -x "$CLASSIC_DIR/aquacrop" ]; then CLASSIC_BIN="$CLASSIC_DIR/aquacrop"; fi
if [ -z "$CLASSIC_BIN" ]; then
  CLASSIC_BIN=$(find "$CLASSIC_DIR" -type f -executable -name "*aquacrop*" 2>/dev/null | head -n1 || true)
fi

if [ -z "$CLASSIC_BIN" ]; then
  echo "[Warn] No executable found for classic AquaCrop. Ensure a binary exists in $CLASSIC_DIR or a recognizable name.
Skipping run."
  exit 0
fi

echo "[Info] Using executable: $CLASSIC_BIN"

## Prepare test directories under /test (absolute) with local fallback if needed
TARGET_TEST_ROOT="/test"
if [ -d "$TARGET_TEST_ROOT" ]; then
  if [ ! -w "$TARGET_TEST_ROOT" ]; then
    echo "[Warn] /test is not writable. Falling back to local test directory."
    TARGET_TEST_ROOT="$ROOT_DIR/test"
  fi
fi
if [ ! -d "$TARGET_TEST_ROOT" ]; then
  mkdir -p "$TARGET_TEST_ROOT"
fi

echo "[Info] Test results will be stored in: $TARGET_TEST_ROOT"

mkdir -p "$TARGET_TEST_ROOT"/case-*/params "$TARGET_TEST_ROOT"/case-*/output

# Initialize some default test cases if none exist
shopt -s nullglob
CASE_DIRS=("$TARGET_TEST_ROOT"/case-*/)
if [ ${#CASE_DIRS[@]} -eq 0 ]; then
  echo "[Info] Creating default test cases: case-01 and case-02"
  mkdir -p "$TARGET_TEST_ROOT/case-01/params" "$TARGET_TEST_ROOT/case-01/input" "$TARGET_TEST_ROOT/case-01/output"
  mkdir -p "$TARGET_TEST_ROOT/case-02/params" "$TARGET_TEST_ROOT/case-02/input" "$TARGET_TEST_ROOT/case-02/output"
  cat > "$TARGET_TEST_ROOT/case-01/params/params.txt" << 'EOF'
param1=default_case01
param2=default
EOF
  cat > "$TARGET_TEST_ROOT/case-02/params/params.txt" << 'EOF'
param1=default_case02
param2=alternate
EOF
fi
set +u

CASE_INDEX=1
for CASE_DIR in "$TARGET_TEST_ROOT"/case-*; do
  [ -d "$CASE_DIR" ] || continue
  CASE_BIN="$CLASSIC_BIN"
  OUT_FILE="$CASE_DIR/output/result.txt"
  LOG_FILE="$CASE_DIR/output/run.log"
  PARAM_FILE="$CASE_DIR/params/params.txt"

  echo "[Info] Running test case: $CASE_DIR"
  mkdir -p "$CASE_DIR/output"

  # If there is an input file, feed it to the executable
  INPUT_FILE="$CASE_DIR/input/input.txt"
  if [ -f "$INPUT_FILE" ]; then
    "$CASE_BIN" < "$INPUT_FILE" > "$OUT_FILE" 2> "$LOG_FILE" || true
  elif [ -f "$PARAM_FILE" ]; then
    # Some classic AquaCrop variants may read parameters from a file; supply as a basic redirection
    "$CASE_BIN" "$PARAM_FILE" > "$OUT_FILE" 2> "$LOG_FILE" || true
  else
    "$CASE_BIN" > "$OUT_FILE" 2> "$LOG_FILE" || true
  fi

  # Persist param file content for later comparison
  if [ -f "$PARAM_FILE" ]; then
    cp "$PARAM_FILE" "$CASE_DIR/params/params.txt"
  fi

  CASE_INDEX=$((CASE_INDEX+1))
done

echo "[Info] Classic AquaCrop run completed. Results stored under $TARGET_TEST_ROOT."
