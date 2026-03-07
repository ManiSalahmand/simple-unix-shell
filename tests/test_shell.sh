#!/bin/sh

set -eu

run_test() {
    name="$1"
    input="$2"
    expected="$3"

    output=$(printf "%b" "$input" | ./build/shell)

    if [ "$output" = "$expected" ]; then
        printf "PASS: %s\n" "$name"
    else
        printf "FAIL: %s\n" "$name"
        printf "Expected: [%s]\n" "$expected"
        printf "Got:      [%s]\n" "$output"
        exit 1
    fi
}

run_test "prompt shown on EOF" "" "shell> "
run_test "whitespace input ignored" "   \n" "shell> shell> "
run_test "non-empty input handled" "ls\n" "shell> shell> "

printf "All integration tests passed.\n"
