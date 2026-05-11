#!/usr/bin/env bash
set -euo pipefail

HUNNU="${HUNNU:-./build/hunnu}"
PASS=0
FAIL=0

green() { printf "\033[32m%s\033[0m\n" "$1"; }
red()   { printf "\033[31m%s\033[0m\n" "$1"; }

run_test() {
    local name="$1"
    local cmd="$2"
    local expected="$3"

    if output=$($cmd 2>&1); then
        if [ -n "$expected" ]; then
            if echo "$output" | grep -qF "$expected"; then
                green "  PASS  $name"
                PASS=$((PASS + 1))
            else
                red "  FAIL  $name (expected output not found)"
                echo "    Expected to contain: $expected"
                echo "    Got: $output"
                FAIL=$((FAIL + 1))
            fi
        else
            green "  PASS  $name"
            PASS=$((PASS + 1))
        fi
    else
        red "  FAIL  $name (exit code $?)"
        echo "    Output: $output"
        FAIL=$((FAIL + 1))
    fi
}

echo "=== Hunnu Test Suite ==="
echo ""

# --- C Interpreter Tests ---
echo "--- C Interpreter ---"
for f in examples/test_*.hn examples/main.hn; do
    if [ ! -f "$f" ]; then continue; fi
    name="$(basename "$f")"
    run_test "run $name" "$HUNNU run $f" ""
done

# --- AOT Compile Tests ---
echo "--- AOT Compiler ---"
for f in examples/main.hn examples/test_oop.hn examples/test_class_top.hn; do
    if [ ! -f "$f" ]; then continue; fi
    name="$(basename "$f")"
    outname="/tmp/hunnu_test_${name%.hn}"
    run_test "compile $name" "$HUNNU compile $f -o $outname && $outname" ""
done

echo ""
echo "=== Results: $PASS passed, $FAIL failed ==="
exit $FAIL
