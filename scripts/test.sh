#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$repo_root"

make clean >/dev/null 2>&1 || true
make >/dev/null 2>&1

output="$(./bin/signal_ipc 2>&1 || true)"

echo "$output"

# Reject starter code that has not been implemented
if echo "$output" | grep -q "TODO:"; then
    echo "visible test FAILED: output contains TODO placeholder text" >&2
    exit 1
fi

expected="worker: ready
supervisor: sending task 1
worker: received task 1
supervisor: sending task 2
worker: received task 2
supervisor: sending task 3
worker: received task 3
supervisor: shutting down worker
worker: all tasks done, exiting
supervisor: worker exited cleanly"

if [[ "$output" != "$expected" ]]; then
    echo "visible test FAILED: output does not match expected" >&2
    echo "--- expected ---" >&2
    echo "$expected" >&2
    echo "--- got ---" >&2
    echo "$output" >&2
    exit 1
fi

echo "visible test: passed"
