#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ASSIGNMENT="$(basename $(dirname "$ROOT_DIR"))"

score=0
max_score=100

pass() {
  echo "[PASS] $1"
}

fail() {
  echo "[FAIL] $1"
  exit 1
}

# Default: run check.sh and grade.sh if present
if [[ -x "$ROOT_DIR/scripts/check.sh" ]]; then
  echo "== Running check.sh =="
  "$ROOT_DIR/scripts/check.sh" || fail "check.sh failed"
  pass "check.sh passed"
  score=$((score + 50))
fi

if [[ -x "$ROOT_DIR/scripts/grade.sh" ]]; then
  echo "== Running grade.sh =="
  "$ROOT_DIR/scripts/grade.sh" || fail "grade.sh failed"
  pass "grade.sh passed"
  score=$((score + 50))
fi

echo "== Final Score: $score/$max_score =="
exit 0
