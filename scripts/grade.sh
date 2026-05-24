#!/usr/bin/env bash
set -euo pipefail

"$(cd "$(dirname "$0")" && pwd)/test.sh"
echo "grade hook: visible checks passed"
