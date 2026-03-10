#!/bin/sh

set -eu

make >/dev/null

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

run_regex_test() {
    name="$1"
    input="$2"
    stdout_pattern="$3"
    stderr_pattern="$4"

    stdout_file=$(mktemp)
    stderr_file=$(mktemp)

    printf "%b" "$input" | ./build/shell >"$stdout_file" 2>"$stderr_file"

    if grep -Eq "$stdout_pattern" "$stdout_file" && grep -Eq "$stderr_pattern" "$stderr_file"; then
        printf "PASS: %s\n" "$name"
    else
        printf "FAIL: %s\n" "$name"
        printf "Expected stdout pattern: [%s]\n" "$stdout_pattern"
        printf "Got stdout:              [%s]\n" "$(cat "$stdout_file")"
        printf "Expected stderr pattern: [%s]\n" "$stderr_pattern"
        printf "Got stderr:              [%s]\n" "$(cat "$stderr_file")"
        rm -f "$stdout_file" "$stderr_file"
        exit 1
    fi

    rm -f "$stdout_file" "$stderr_file"
}

current_dir=$(pwd)

run_split_test "prompt shown on EOF" "" "shell> " ""
run_split_test "whitespace input ignored" "   \n" "shell> shell> " ""
run_split_test "echo command executes" "echo hello\n" "shell> hello
shell> " ""
run_split_test "echo command with arguments executes" "echo hello world\n" "shell> hello world
shell> " ""
run_split_test "pwd command executes" "pwd\n" "shell> $current_dir
shell> " ""
run_split_test "multiple commands execute in sequence" "echo first\necho second\n" "shell> first
shell> second
shell> " ""
run_regex_test "invalid command prints error and reprompts" "notacommand\n" '^shell> shell> $' '^notacommand: .+$'

printf "All integration tests passed.\n"
