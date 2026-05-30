*This project has been created as part of the 42 curriculum by hbani-at, amufleh.*

# ShellWeREPLit (minishell)

> A small, bash-like Unix shell written in C — your very own **R**ead-**E**val-**P**rint **L**oop for the terminal.

---

## Description

**ShellWeREPLit** is a custom, lightweight Unix shell interpreter developed in C,
designed to emulate the core behaviour of `bash`. The program displays a prompt,
reads a line of input, parses it into commands, expands variables and quotes,
sets up the requested redirections and pipes, then executes the resulting command
pipeline — looping forever until the user exits. In short, it is a complete
**R**ead–**E**val–**P**rint **L**oop built on top of raw Unix system calls.

The goal of the project is to understand, from the ground up, what really happens
when you type a command and press *Enter*: how a line of text is tokenized, how the
shell resolves an executable through `$PATH`, how processes are created with
`fork`, replaced with `execve`, connected with `pipe`/`dup2`, and reaped with
`waitpid`, and how signals interact with an interactive terminal session.

### Feature overview

- **Interactive prompt** with command history (via the GNU `readline` library).
- **Tokenizer + parser** that turns a raw line into a linked list of commands.
- **Quote handling** — single quotes (`'…'`) preserve everything literally; double
  quotes (`"…"`) preserve everything except variable expansion.
- **Environment variable expansion** — `$VAR`, `$?` (last exit status), and edge
  cases inside/outside quotes.
- **Redirections** — input `<`, output `>`, append `>>`, and here-documents `<<`
  (with delimiter quoting controlling whether the body is expanded).
- **Pipelines** — arbitrarily long `cmd1 | cmd2 | cmd3 …`.
- **Built-in commands** — `echo` (with `-n`), `cd`, `pwd`, `export`, `unset`,
  `env`, and `exit`.
- **Signal handling** that mirrors bash:
  - `Ctrl-C` (`SIGINT`) — new prompt line, exit status `130`.
  - `Ctrl-\` (`SIGQUIT`) — ignored in interactive mode.
  - `Ctrl-D` (EOF) — exits the shell cleanly.
- **Exit-status propagation** consistent with bash conventions.
- Memory-leak conscious — ships with a valgrind suppression file (`supp.supp`).

---

## Repository structure

```
ShellWeREPLit/
├── main.c                 # Entry point + the REPL read loop
├── error.c                # Error reporting helpers
├── include/
│   ├── parsing.h          # Tokenizer / parser / expansion types & prototypes
│   └── execution.h        # Execution, env, builtins & signal prototypes
├── parsing/               # Lexer, parser, quote/variable expansion, heredoc
├── execution/             # Process creation, pipelines, redirections, env mgmt
├── builtin_funcitons/     # echo, cd, pwd, export, unset, env, exit
├── signal_handling/       # SIGINT / SIGQUIT handlers
├── libft/                 # 42 custom C standard-library reimplementation
├── Makefile               # Build rules
└── supp.supp              # valgrind suppression file (for readline noise)
```

---

## Requirements

To build and run the project you need:

| Dependency          | Why it is needed                                              |
| ------------------- | ------------------------------------------------------------ |
| `cc` / `gcc`        | C compiler (project compiles with `-Wall -Werror -Wextra`)   |
| `make`              | Drives the build                                             |
| **GNU readline**    | Interactive line editing + history (linked with `-lreadline`) |
| A Unix-like OS      | Linux or macOS (uses POSIX system calls)                     |
| `valgrind` *(opt.)* | Memory-leak checking                                         |

### Installing readline

```bash
# Debian / Ubuntu
sudo apt-get install libreadline-dev

# Fedora
sudo dnf install readline-devel

# macOS (Homebrew)
brew install readline
```

---

## Instructions

### Compilation

```bash
git clone <repository-url> ShellWeREPLit
cd ShellWeREPLit
make
```

The `Makefile` first builds the `libft` archive (`libft/libft.a`), compiles every
source file, then links everything together with `-lreadline` to produce the
`minishell` executable.

Available `make` targets:

| Target        | Effect                                          |
| ------------- | ----------------------------------------------- |
| `make`        | Build the `minishell` executable (default `all`) |
| `make clean`  | Remove object files                             |
| `make fclean` | Remove object files **and** the executable      |
| `make re`     | `fclean` followed by a full rebuild             |

### Execution

```bash
./minishell
```

The program takes **no command-line arguments** — passing any will print an error
and exit. Once running, type commands at the `minishell$ ` prompt exactly as you
would in bash.

### Usage examples

```bash
minishell$ echo "Hello, $USER!"
Hello, hbani-at!

minishell$ ls -la | grep ".c" | wc -l
12

minishell$ export GREETING=hi
minishell$ echo $GREETING
hi

minishell$ cat << EOF > out.txt
> line one
> line two
> EOF
minishell$ cat < out.txt
line one
line two

minishell$ cd /tmp && pwd
/tmp

minishell$ false
minishell$ echo $?
1

minishell$ exit
```

### Testing for memory leaks

The bundled suppression file silences known one-time allocations from the
`readline` library:

```bash
valgrind --leak-check=full --show-leak-kinds=all \
         --suppressions=supp.supp --trace-children=yes \
         --track-fds=yes ./minishell
```

---

## Technical overview

### The REPL pipeline

```
        ┌─────────────┐   ┌────────────┐   ┌─────────────┐   ┌──────────────┐
input → │  Tokenizer  │ → │   Parser   │ → │  Expansion  │ → │  Execution   │ → output
        │  (lexer)    │   │ (commands) │   │ (vars/quote)│   │ (fork/exec)  │
        └─────────────┘   └────────────┘   └─────────────┘   └──────────────┘
              ▲                                                      │
              └──────────────────── loop until EOF / exit ──────────┘
```

1. **Read** — `readline()` prints the prompt and returns the typed line; the line
   is added to history.
2. **Tokenize** — `parsing/tokenizer.c` splits the line into a doubly-linked list
   of tokens (`t_tokens`), classifying each as a word, pipe, or redirection
   operator (`<`, `>`, `>>`, `<<`).
3. **Validate syntax** — `parsing/syntax_validation.c` rejects malformed input
   (e.g. a pipe with nothing after it).
4. **Parse** — `parsing/parser.c` groups tokens into a list of commands
   (`t_commands`), each with its `argv` and its redirection list (`t_redir`).
5. **Expand** — `parsing/command_expansion*.c` resolves `$VAR`, `$?`, and applies
   quote rules.
6. **Heredocs** — `parsing/heredoc.c` collects `<<` bodies before execution.
7. **Execute** — `execution/` either runs a single built-in in the parent process
   or forks a pipeline, wiring `pipe()`/`dup2()` between stages and calling
   `execve()` on the resolved binary path. Exit statuses are collected with
   `waitpid()`.

### Key data structures (`include/parsing.h`)

- `t_tokens` — a lexer token (type + value, doubly linked).
- `t_redir` — one redirection (type, target file, optional heredoc fd).
- `t_commands` — one command in a pipeline (`argv` + its redirections).
- `t_envp` — the environment stored as a key/value linked list.
- `t_program_info` — the global program state: environment, command list, the last
  exit status, and saved copies of the original stdin/stdout.

### Built-in commands

| Built-in | File                                 | Notes                                  |
| -------- | ------------------------------------ | -------------------------------------- |
| `echo`   | `builtin_funcitons/echo_builtin.c`   | Supports the `-n` flag                 |
| `cd`     | `builtin_funcitons/cd_builtin.c`     | Uses `chdir()`, updates `PWD`/`OLDPWD` |
| `pwd`    | `builtin_funcitons/pwd_builtin.c`    | Uses `getcwd()`                        |
| `export` | `builtin_funcitons/export_builtin.c` | Adds/updates env variables             |
| `unset`  | `builtin_funcitons/unset_builtin.c`  | Removes env variables                  |
| `env`    | `builtin_funcitons/env_builtin.c`    | Prints the environment                 |
| `exit`   | `builtin_funcitons/exit_builtin.c`   | Leaves the shell with a status code    |

Built-ins are dispatched in `execution/builtins_dispatch.c`. A built-in that is not
part of a pipeline runs in the parent process so that state changes (like `cd` or
`export`) persist.

---

## System calls & library functions used

The shell is built directly on POSIX primitives. The most important ones:

### Process management
- **`fork`** — create a child process for each external command in a pipeline.
- **`execve`** — replace the child image with the target executable.
- **`wait` / `waitpid`** — reap children and collect their exit status.
- **`exit`** — terminate a process with a status code.

### File descriptors & redirection
- **`pipe`** — create the connected fd pairs that link pipeline stages.
- **`dup` / `dup2`** — redirect stdin/stdout onto files or pipe ends.
- **`open` / `close`** — open redirection target files / tidy up fds.
- **`read` / `write`** — low-level I/O (e.g. writing to stdout in signal handlers).
- **`access`** — check whether a candidate `$PATH` entry is executable.

### Working directory
- **`chdir`** — implement the `cd` built-in.
- **`getcwd`** — implement `pwd` and update `PWD`.

### Signals
- **`signal`** — ignore `SIGQUIT` in interactive mode.
- **`sigaction`** — install the `SIGINT` handlers (interactive + heredoc modes).
- **`sigemptyset`** — initialise the signal mask.

### Errors
- **`strerror` / `perror`** — turn `errno` into human-readable diagnostics.

### GNU readline library
- **`readline`** — print the prompt and read a full line with editing.
- **`add_history`** — store entered lines for up-arrow recall.
- **`rl_clear_history`** — free history on exit.
- **`rl_on_new_line` / `rl_replace_line` / `rl_redisplay`** — redraw a fresh prompt
  after `Ctrl-C`.

---

## Relevant man pages

While exploring or extending the project, these manual pages are the canonical
references:

```
man 3 readline       man 2 fork          man 2 execve
man 2 pipe           man 2 dup           man 2 dup2
man 2 wait           man 2 waitpid       man 2 open
man 2 close          man 2 read          man 2 write
man 2 access         man 2 chdir         man 3 getcwd
man 2 signal         man 2 sigaction     man 7 signal
man 3 strerror       man 3 perror        man 3 history
man 1 bash           man 7 environ
```

---

## Resources

Classic references used while building the project:

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) — the behaviour we emulate.
- [Shell (computing) — Wikipedia](https://en.wikipedia.org/wiki/Shell_(computing))
- [Shell builtin — Wikipedia](https://en.wikipedia.org/wiki/Shell_builtin)
- [Process (computing) — Wikipedia](https://en.wikipedia.org/wiki/Process_(computing))
- [REPL — Wikipedia](https://en.wikipedia.org/wiki/Read%E2%80%93eval%E2%80%93print_loop)
- [Behind the scenes: what happens when you execute a command in the shell?](https://medium.com/@SergioPietri/behind-the-scenes-what-happens-when-you-execute-a-command-in-the-shell-9fef53832f6a)
- [Minishell: Building a mini-bash — a 42 project](https://m4nnb3ll.medium.com/minishell-building-a-mini-bash-a-42-project-b55a10598218)
- [Building a Parser from scratch (video)](https://www.youtube.com/watch?v=4m7ubrdbWQU&t=38s)
- [Heredoc: A Deep Dive](https://medium.com/@oduwoledare/heredoc-a-deep-dive-23c82992e522)
- [How to distinguish between a builtin and an external util?](https://unix.stackexchange.com/questions/371722/how-to-distinguish-between-builtin-and-external-util-e-g-echo)
- [42-minishell explained (GitHub)](https://github.com/multitudes/42-minishell)
- [`dup()` and `dup2()` Linux system call](https://www.geeksforgeeks.org/c/dup-dup2-linux-system-call/)
- [Ctrl-D is like Enter](https://hackarcana.com/article/ctrl-d-is-like-enter)
- [What does Ctrl-D mean on Unix/Linux systems?](https://www.quora.com/What-does-Ctrl-D-mean-on-Unix-Linux-systems)

### Use of AI

AI tools were used as an **assistant**, not as a replacement for our own work, on
the following tasks:

- **Documentation** — drafting and structuring this `README.md` from the project's
  actual source code, Makefile, and headers.
- **Debugging help** — explaining confusing edge cases (signal behaviour inside
  here-documents, quote/expansion interaction) and suggesting test cases.
- **Code review & refactoring** — pointing out norm violations, dead code, and
  clearer ways to split functions across files.

All architectural decisions, the parsing/execution design, and the final
implementation were written, reviewed, and validated by the authors.

---

## Authors

- **hbani-at** — *42 login*
- **amufleh** — *42 login*

Made with C, curiosity, and a lot of `fork()`-ing around. 🐚
