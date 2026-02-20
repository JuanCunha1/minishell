*This project has been created as part of the 42 curriculum by jmarques, xlamiel-.*

# Minishell

## Description

**Minishell** is a simplified Unix shell written in C, developed as part of the 42 curriculum.
The goal of this project is to recreate the core behavior of a Bash-like shell capable of reading, parsing, and executing commands entered by the user.

The objectives of this project are to:

* Display a prompt and wait for user input
* Execute commands with arguments using the system PATH
* Handle pipes (`|`) between multiple commands
* Implement input and output redirections (`<`, `>`, `>>`, `<<`)
* Expand environment variables (`$VAR`) and the last exit status (`$?`)
* Implement built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`
* Correctly manage signals such as `Ctrl+C`, `Ctrl+D`, and `Ctrl+\`

This project demonstrates skills in **process management, file descriptors, parsing, memory management, signals handling, and environment manipulation**, while respecting the constraints defined in the 42 subject.

---

## Project Architecture

The project is organized into the following modules:

* **include/**

  * Header files defining shared structures and function prototypes (`minishell.h`, `lexer.h`).

* **libft/**

  * Custom utility library used throughout the project (string handling, memory, lists, printf, etc.).

* **src/main.c**

  * Entry point of the program.
  * Initializes the shell, environment, and main execution loop.

* **src/global_utils/**

  * Shell initialization, environment management, and memory cleanup helpers.

* **src/lexer/**

  * Converts user input into tokens.
  * Handles quoting, tokenizer states, and environment variable expansion.

* **src/syntax_error/**

  * Checks tokenized input for syntax errors before the parser builds the command structures.

* **src/parser/**

  * Builds internal command structures (AST) from tokens.
  * Processes pipes, redirections, and heredocs.

* **src/exec/**

  * Executes parsed commands.
  * Handles pipes, redirections, heredocs, path resolution, and process creation.

* **src/builtins/**

  * Implementation of built-in shell commands (`cd`, `echo`, `env`, `exit`, `export`, `pwd`, `unset`).

* **src/signal/**

  * Manages signal behavior for interactive shell execution.

Overall flow:

**Input → Lexer → Syntax Check → Parser/AST → Executor/Builtins → Cleanup → Next Prompt**

---

## Instructions

### Compilation

Clone the repository and compile the project:

```bash
git clone <repository_url>
cd minishell
make
```

Useful Makefile rules:

```bash
make        # Compile the project
make clean  # Remove object files
make fclean # Remove object files and executable
make re     # Recompile everything
```

### Execution

Run the shell with:

```bash
./minishell
```

Example commands inside the shell:

```bash
echo Hello World
ls -l | grep ".c"
export MY_VAR=42
echo $MY_VAR
cat < infile.txt >> outfile.txt
```

### Requirements

* **Language:** C
* **Compiler:** gcc or clang
* **Operating System:** Linux or macOS
* **Allowed functions:** Standard C library and POSIX system calls (as specified in the subject)

---

## Resources

### Documentation & References

* Unix manual pages (`man bash`, `man fork`, `man execve`, `man pipe`, `man dup2`, `man signal`)
* POSIX Shell Command Language specification
* GNU Bash documentation
* 42 Minishell subject and intra materials
* Tutorials on Unix processes, pipes, and file descriptor handling

### AI Usage

AI tools were used for:

* Clarifying shell behavior and parsing logic
* Understanding system calls and signal handling
* Discussing debugging approaches and possible edge cases

All design decisions, implementation, testing, and validation were performed manually by the authors to ensure compliance with 42 project rules.
