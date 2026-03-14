#!/bin/sh

set -eu

make >/dev/null

TMP_FILES=""

create_temp_file() {
    file=$(mktemp)
    TMP_FILES="$TMP_FILES $file"
    printf "%s" "$file"
}

cleanup() {
    rm -f $TMP_FILES
}

trap cleanup EXIT INT TERM

run_shell() {
    input="$1"
    stdout_file="$2"
    stderr_file="$3"

    printf "%b" "$input" | ./build/shell >"$stdout_file" 2>"$stderr_file"
}

print_failure() {
    name="$1"
    expected_stdout="$2"
    actual_stdout="$3"
    expected_stderr="$4"
    actual_stderr="$5"

    printf "FAIL: %s\n" "$name"
    printf "Expected stdout: [%s]\n" "$expected_stdout"
    printf "Got stdout:      [%s]\n" "$actual_stdout"
    printf "Expected stderr: [%s]\n" "$expected_stderr"
    printf "Got stderr:      [%s]\n" "$actual_stderr"
    exit 1
}

run_split_test() {
    name="$1"
    input="$2"
    expected_stdout="$3"
    expected_stderr="$4"

    stdout_file=$(create_temp_file)
    stderr_file=$(create_temp_file)

    run_shell "$input" "$stdout_file" "$stderr_file"

    actual_stdout=$(cat "$stdout_file")
    actual_stderr=$(cat "$stderr_file")

    if [ "$actual_stdout" = "$expected_stdout" ] &&
       [ "$actual_stderr" = "$expected_stderr" ]; then
        printf "PASS: %s\n" "$name"
    else
        print_failure \
            "$name" \
            "$expected_stdout" \
            "$actual_stdout" \
            "$expected_stderr" \
            "$actual_stderr"
    fi
}

run_regex_test() {
    name="$1"
    input="$2"
    stdout_pattern="$3"
    stderr_pattern="$4"

    stdout_file=$(create_temp_file)
    stderr_file=$(create_temp_file)

    run_shell "$input" "$stdout_file" "$stderr_file"

    if grep -Eq "$stdout_pattern" "$stdout_file" &&
       grep -Eq "$stderr_pattern" "$stderr_file"; then
        printf "PASS: %s\n" "$name"
    else
        print_failure \
            "$name" \
            "$stdout_pattern" \
            "$(cat "$stdout_file")" \
            "$stderr_pattern" \
            "$(cat "$stderr_file")"
    fi
}

home_dir=${HOME:-}
current_dir=$(pwd)

redir_input_file=$(create_temp_file)
redir_output_file=$(create_temp_file)
missing_input_file="/tmp/simple-unix/shell-missing-$$"

printf "banana\napple\ncarrot\n" >"$redir_input_file"
printf "old content\n" >"$redir_output_file"
rm -f "$missing_input_file"

run_split_test "prompt shown on EOF" "" "shell> " ""
run_split_test "whitespace input ignored" "   \n" "shell> shell> " ""

run_split_test "echo command executes" \
"echo hello\n" \
"shell> hello
shell> " \
""

run_split_test "echo command with arguments executes" \
"echo hello world\n" \
"shell> hello world
shell> " \
""

run_split_test "pwd command executes" \
"pwd\n" \
"shell> $current_dir
shell> " \
""

run_split_test "multiple commands execute in sequence" \
"echo first\necho second\n" \
"shell> first
shell> second
shell> " \
""

run_split_test "cd changes working directory for subsequent commands" \
"cd /
pwd
" \
"shell> shell> /
shell> " \
""

run_split_test "cd with too many arguments prints error" \
"cd / tmp
" \
"shell> shell> " \
"shell: cd: too many arguments"

if [ -n "$home_dir" ]; then
    run_split_test "cd with no arguments uses HOME" \
"cd
pwd
" \
"shell> shell> $home_dir
shell> " \
""
fi

run_split_test "exit builtin terminates shell" \
"exit
" \
"shell> " \
""

run_split_test "exit with too many arguments prints error" \
"exit now
exit
" \
"shell> shell> " \
"shell: exit: too many arguments"

run_split_test "input redirection with cat" \
"cat < $redir_input_file
" \
"shell> banana
apple
carrot
shell> " \
""

run_split_test "input redirection with sort" \
"sort < $redir_input_file
" \
"shell> apple
banana
carrot
shell> " \
""

run_split_test "output redirection writes to file" \
"echo hello > $redir_output_file
cat $redir_output_file
" \
"shell> shell> hello
shell> " \
""

run_split_test "output redirection truncates existing file" \
"echo new > $redir_output_file
cat $redir_output_file
" \
"shell> shell> new
shell> " \
""

run_split_test "combined input and output redirection" \
"sort < $redir_input_file > $redir_output_file
cat $redir_output_file
" \
"shell> shell> apple
banana
carrot
shell> " \
""

run_split_test "missing filename after input redirection is ignored safely" \
"cat <
" \
"shell> shell> " \
"shell: parser: missing filename after '<'"

run_split_test "missing filename after output redirection is ignored safely" \
"echo hello >
" \
"shell> shell> " \
"shell: parser: missing filename after '>'"

run_regex_test "invalid command prints error and reprompts" \
"notacommand\n" \
'^shell> shell> $' \
'^notacommand: .+$'

run_regex_test "cd invalid path prints error and reprompts" \
"cd /definitely/not/a/real/path
" \
'^shell> shell> $' \
'^cd: .+$'

run_regex_test "input redirection missing file prints error" \
"cat < $missing_input_file
" \
'^shell> shell> $' \
'^open: .+$'

pytest

printf "All integration tests passed.\n"