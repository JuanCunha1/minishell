#!/usr/bin/env bash

MINISHELL=./minishell

RED="\033[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[1;33m"
RESET="\033[0m"

pass=0
fail=0

normalize() {
    # remove shell prefixes and trailing spaces
    sed 's/^[^:]*: //; s/[[:space:]]*$//'
}

run_test() {
    cmd="$1"

    bash_out=$(bash -c "$cmd" 2>&1 | normalize)
    bash_status=$?

    mini_out=$(printf "%s\nexit\n" "$cmd" | $MINISHELL 2>&1 | sed '$d' | normalize)
    mini_status=$?

    if [[ "$bash_out" == "$mini_out" && "$bash_status" == "$mini_status" ]]; then
        echo -e "${GREEN}PASS${RESET} : $cmd"
        ((pass++))
    else
        echo -e "${RED}FAIL${RESET} : $cmd"
        echo -e "${YELLOW}Command:${RESET} $cmd"
        echo "--- bash output ---"
        echo "$bash_out"
        echo "--- minishell output ---"
        echo "$mini_out"
        echo "bash status: $bash_status"
        echo "mini status: $mini_status"
        echo "------------------------"
        ((fail++))
    fi
}

echo "============================"
echo "MINISHELL TESTER"
echo "============================"
echo

# BASIC
run_test "echo hello"
run_test "echo hello world"
run_test "pwd"
run_test "ls"
run_test "whoami"

# ARGUMENTS
run_test "echo a b c d"
run_test "ls -l"
run_test "ls -la"

# QUOTES
run_test "echo 'hello world'"
run_test "echo \"hello world\""
run_test "echo \"hello     world\""
run_test "echo 'hello     world'"
run_test "echo \"hello 'world'\""
run_test "echo 'hello \"world\"'"

# ENV
run_test "echo \$HOME"
run_test "echo \$PATH"
run_test "echo \$USER"
run_test "echo \$NOTEXIST"

# COMMAND NOT FOUND
run_test "fewqfewqfwefqewfewq"
run_test "notacommand"

# REDIRECTION >
run_test "echo hello > file1"
run_test "cat file1"

# REDIRECTION >>
run_test "echo hello > file2"
run_test "echo world >> file2"
run_test "cat file2"

# INPUT REDIRECTION
echo "hello file" > infile
run_test "cat < infile"
run_test "wc -l < infile"

# REDIRECTION ERRORS
run_test "cat < non_existing"
run_test "cat < non_existing | wc"

# MULTIPLE REDIRS
run_test "echo hello > a > b > c"
run_test "cat c"

# BASIC PIPE
run_test "echo hello | cat"
run_test "echo hello world | wc -w"
run_test "ls | wc -l"

# MULTIPLE PIPE
run_test "echo hello world | tr ' ' '\n' | wc -l"
run_test "ls | grep . | wc -l"
run_test "echo hello | cat | cat | wc -l"

# PIPE + REDIR
run_test "echo hello | cat > out1"
run_test "cat out1"

# PIPE + INPUT
run_test "cat < infile | wc -l"

# COMPLEX PIPELINES
run_test "echo hello world | tr ' ' '\n' | sort | wc -l"
run_test "ls | sort | head -n 3 | wc -l"

# SPACING
run_test "   ls"
run_test "ls     "
run_test "echo     hello"
run_test "echo hello     world"

# EDGE CASES
run_test "echo ''"
run_test "echo \"\""
run_test "echo \$"

# PATH
run_test "/bin/ls"

# REDIRECTION + PIPE CHAINS
run_test "echo hello > t1"
run_test "cat t1 | wc -l"
run_test "cat < t1 | wc -c"
run_test "cat < t1 | cat | wc"

# MORE REDIR EDGE CASES
run_test "echo hello > x > y"
run_test "cat y"
run_test "cat < x < y"

# LONG PIPELINE
run_test "echo one two three four | tr ' ' '\n' | grep o | sort | wc -l"

# LARGE ARGUMENT LIST
run_test "echo a b c d e f g h i j k l m n o p"

# FILE PERMISSIONS TEST
touch permtest
chmod 000 permtest
run_test "cat permtest"
chmod 644 permtest

# BASIC COMMANDS
run_test "echo hello"
run_test "echo hello world"
run_test "pwd"
run_test "ls"
run_test "whoami"

# ARGUMENTS
run_test "echo a b c d"
run_test "ls -l"
run_test "ls -la"

# QUOTES
run_test "echo 'hello world'"
run_test "echo \"hello world\""
run_test "echo \"hello     world\""
run_test "echo 'hello     world'"

# ENV
run_test "echo \$HOME"
run_test "echo \$PATH"
run_test "echo \$USER"

# COMMAND NOT FOUND
run_test "abcdefvfdavfdv"
run_test "notacommand"

# REDIRECTION >
run_test "echo hello > file1"
run_test "cat file1"

# REDIRECTION >>
run_test "echo hello > file2"
run_test "echo world >> file2"
run_test "cat file2"

# INPUT REDIRECTION
echo "hello file" > infile
run_test "cat < infile"
run_test "wc -l < infile"

# REDIRECTION ERRORS
run_test "cat < non_existing"
run_test "cat < non_existing | wc"

# MULTIPLE REDIRS
run_test "echo hello > a > b > c"
run_test "cat c"

# BASIC PIPE
run_test "echo hello | cat"
run_test "echo hello world | wc -w"
run_test "ls | wc -l"

# MULTIPLE PIPE
run_test "echo hello world | tr ' ' '\n' | wc -l"
run_test "ls | grep . | wc -l"

# PIPE + REDIR
run_test "echo hello | cat > out1"
run_test "cat out1"

# PIPE + INPUT
run_test "cat < infile | wc -l"

# SPACING
run_test "   ls"
run_test "ls     "
run_test "echo     hello"

# COMPLEX
run_test "echo hello world | tr ' ' '\n' | sort | wc -l"

# EDGE CASES
run_test "echo ''"
run_test "echo \"\""
run_test "echo \$NOTEXIST"

# PATH TEST
run_test "/bin/ls"


echo
echo "============================"
echo "Tests Passed: $pass"
echo "Tests Failed: $fail"
echo "============================"
