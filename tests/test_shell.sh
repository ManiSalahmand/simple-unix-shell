#!/bin/sh

set -eu

run_split_test() {
    name="$1"
    input="$2"
    expected_stdout="$3"
    expected_stderr="$4"

    stdout_file=$(mktemp)
    stderr_file=$(mktemp)

    printf "%b" "$input" | ./build/shell >"$stdout_file" 2>"$stderr_file"

    actual_stdout=$(cat "$stdout_file")
    actual_stderr=$(cat "$stderr_file")

    rm -f "$stdout_file" "$stderr_file"

    if [ "$actual_stdout" = "$expected_stdout" ] && [ "$actual_stderr" = "$expected_stderr" ]; then
        printf "PASS: %s\n" "$name"
    else
        printf "FAIL: %s\n" "$name"
        printf "Expected stdout: [%s]\n" "$expected_stdout"
        printf "Got stdout:      [%s]\n" "$actual_stdout"
        printf "Expected stderr: [%s]\n" "$expected_stderr"
        printf "Got stderr:      [%s]\n" "$actual_stderr"
        exit 1
    fi
}

run_split_test "prompt shown on EOF" "" "shell> " ""
run_split_test "whitespace input ignored" "   \n" "shell> shell> " ""
run_split_test "non-empty input reprompts without stderr" "ls\n" "shell> shell> " ""
run_split_test "multiple commands reprompt" "ls\npwd\n" "shell> shell> shell> " ""

printf "All integration tests passed.\n"
