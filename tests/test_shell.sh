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

home_dir=${HOME:-}
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
run_split_test "cd changes working directory for sobsequent commands" "cd /
pwd
" "shell> shell> /
shell> " ""
run_split_test "cd with too many arguments prints error" "cd / tmp
" "shell> shell> " "shell: cd: too many arguments"
if [ -n "$home_dir" ]; then
    run_split_test "cd with no arguments uses HOME" "cd
pwd
" "shell> shell> $home_dir
shell> " ""
fi
run_split_test "exit builtin terminates shell" "exit
" "shell> " ""
run_split_test "exit with too many arguments prints error" "exit now
exit
" "shell> shell> " "shell: exit: too many arguments"

run_regex_test "invalid command prints error and reprompts" "notacommand\n" '^shell> shell> $' '^notacommand: .+$'
run_regex_test "cd invalid path prints error and reprompts" "cd /definitely/not/a/real/path
" '^shell> shell> $' '^cd: .+$'

pytest

printf "All integration tests passed.\n"
