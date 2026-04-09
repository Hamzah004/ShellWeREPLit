---
id: 1775594506-XQQV
aliases:
  - minishell plan
tags: []
---

# minishell plan


# Minishell Execution Implementation Plan

## Context

The minishell parsing pipeline (tokenizer, parser, syntax validation, expansion) is complete. The execution side currently only handles single external commands with basic redirections (`<`, `>`, `>>`). Everything else -- pipes, builtins, heredoc, signals, `$?`, environment management -- needs to be built. This plan covers the full execution implementation required by the 42 subject.

---

## Phase 0: Data Structure & Header Changes

**Why first**: Every subsequent phase depends on these foundations.

### `/home/liva/minishell/include/execution.h`
- Add `int exit_status;` to `t_program_info` (tracks `$?`)
- Add `extern volatile sig_atomic_t g_signal;` (the ONE global variable)
- Add `#include <signal.h>`
- Remove unused `t_env` struct (or keep if planned for later)
- Add prototypes incrementally as each phase is implemented

### `/home/liva/minishell/include/parsing.h`
- Add `int heredoc_fd;` to `t_redir` (default -1, stores pre-opened heredoc pipe fd)
- Initialize it to -1 in `new_redirection()` (`parsing/parser_utils.c`)

### `/home/liva/minishell/main.c`
- Define `volatile sig_atomic_t g_signal = 0;`
- Initialize `info.exit_status = 0;` in `main()`

### Parsing coordination: `$?` support
- **File**: `/home/liva/minishell/parsing/command_expansion_utils.c` line 57-83
- `handle_var()` only matches `ft_isalnum || '_'` so `$?` expands to nothing
- **Fix**: After `(*i)++` (line 64), add: if `input[*i] == '?'`, return `ft_itoa(exit_status)` and `(*i)++`
- This requires passing `exit_status` through the expansion chain, or changing `fill_command_struct` to accept `t_program_info *info` instead of `char **env`

### Parsing coordination: live environment
- **File**: `/home/liva/minishell/parsing/fill_command.c` line ~40
- Currently: `fill_command_struct(char *input, char **env)` receives original env
- **Fix**: Pass `info->envp` from `read_from_prompt()` so `export`/`unset` changes are visible to expansion

---

## Phase 1: Environment Management Utilities

**Why second**: Builtins (export, unset, cd) all need these.

### New file: `execution/env_utils.c`
| Function | Purpose |
|----------|---------|
| `char *get_env_var(char **envp, char *name)` | Returns pointer to value after `=`, no alloc |
| `int set_env_var(t_program_info *info, char *name, char *value)` | Add or replace env var, realloc envp |
| `int unset_env_var(t_program_info *info, char *name)` | Remove var, shift array down |
| `int env_index(char **envp, char *name)` | Index of `name=...` or -1 |
| `int is_valid_identifier(char *str)` | Valid var name check (alpha/_ start, alnum/_ body) |

### New file: `execution/env_utils2.c` (if norm requires split)
| Function | Purpose |
|----------|---------|
| `void update_path(t_program_info *info)` | Re-extract and re-split PATH after export/unset |

---

## Phase 2: Builtin Commands

**Why third**: Can be tested independently with current single-fork execution.

### `execution/builtin_functions.c` -- dispatcher + echo + pwd
| Function | Purpose |
|----------|---------|
| `int is_builtin(char *cmd)` | Returns 1 if builtin, 0 if not (fix current inverted logic) |
| `int execute_builtin(t_commands *cmd, t_program_info *info)` | Dispatch to correct builtin, return exit status |
| `int builtin_echo(t_commands *cmd)` | Handle `-n` flag, print args space-separated |
| `int builtin_pwd(void)` | `getcwd()` + print |

### `execution/builtin_cd.c`
| Function | Purpose |
|----------|---------|
| `int builtin_cd(t_commands *cmd, t_program_info *info)` | `chdir()`, handle no-arg (go to $HOME), update PWD/OLDPWD |

### `execution/builtin_export.c`
| Function | Purpose |
|----------|---------|
| `int builtin_export(t_commands *cmd, t_program_info *info)` | No args = print sorted env; with args = set vars |
| `static void print_sorted_env(char **envp)` | Print as `declare -x NAME="VALUE"` sorted |
| `static int parse_export_arg(char *arg, char **name, char **value)` | Split at first `=` |
| `static void sort_env_ptrs(char **arr, int len)` | Simple sort for display |

### `execution/builtin_unset.c`
| Function | Purpose |
|----------|---------|
| `int builtin_unset(t_commands *cmd, t_program_info *info)` | Validate names, call `unset_env_var` |

### `execution/builtin_exit.c`
| Function | Purpose |
|----------|---------|
| `int builtin_exit(t_commands *cmd, t_program_info *info)` | Print "exit", validate arg, exit with status |
| `static int is_numeric_str(char *str)` | Check valid integer string |

### `execution/builtin_env.c`
| Function | Purpose |
|----------|---------|
| `int builtin_env(t_program_info *info)` | Print all env vars containing `=` |

### Key rules for builtins
- **cd, export, unset, exit**: MUST run in parent process (when single command, no pipe)
- **In a pipeline**: ALL builtins run in forked child (their side effects don't affect parent)
- **echo, pwd, env**: Can run in either context

---

## Phase 3: Heredoc (`<<`)

### New file: `execution/heredoc.c`
| Function | Purpose |
|----------|---------|
| `int handle_heredoc(t_redir *redir, t_program_info *info)` | Create pipe, read lines until delimiter, store read-end in `redir->heredoc_fd` |
| `static int read_heredoc_lines(int write_fd, char *delim, int expand, t_program_info *info)` | Read via `readline("> ")`, write to pipe, expand $VAR if unquoted delimiter |
| `static int delimiter_is_quoted(char *delim)` | Check if original delimiter had quotes |
| `static char *expand_heredoc_line(char *line, t_program_info *info)` | Expand $VAR and $? in heredoc content |

### Modify: `execution/handle_redir.c`
- Add `TOK_HEREDOC` case in `apply_redir()`: `dup2(redir->heredoc_fd, STDIN_FILENO)`

### Design decisions
- Use pipe (not temp file) to avoid cleanup issues
- All heredocs in a pipeline are collected BEFORE any command forks (bash behavior)
- Ctrl+C during heredoc cancels it and returns to prompt (signal integration in Phase 5)

---

## Phase 4: Pipeline Execution (Core Rewrite)

**The biggest phase.** Rewrite the execution orchestrator.

### Reorganize `execution/execution.c`

Move init functions to a new file to stay within norm limits:

### New file: `execution/init_env.c` (moved from execution.c)
| Function | Purpose |
|----------|---------|
| `int count_lines(char **env)` | Count env array length |
| `t_error get_env(t_program_info *info, char **env)` | Copy env to info->envp |
| `t_error get_path(t_program_info *info)` | Extract and split PATH |
| `char *get_cmd_path(t_commands *cmd, t_program_info *info)` | Resolve command path |

### Rewritten `execution/execution.c`
| Function | Purpose |
|----------|---------|
| `int execution(t_program_info *info)` | Main orchestrator: count cmds, process heredocs, dispatch to single-builtin or pipeline |
| `static int count_cmds(t_commands *cmd)` | Count commands in linked list |
| `static int process_all_heredocs(t_commands *cmd, t_program_info *info)` | Pre-read all heredocs before forking |
| `void execute_single_cmd(t_program_info *info)` | Existing: resolve path + execve (used in child) |

### New file: `execution/pipeline.c`
| Function | Purpose |
|----------|---------|
| `void run_pipeline(t_program_info *info, int cmd_count)` | Fork each command, wire pipes between them |
| `int wait_for_children(int *pids, int count)` | waitpid all, return last command's exit status |
| `static void child_pipe_setup(int prev_fd, int *pipe_fd, int is_last)` | dup2 pipe fds in child |
| `static void child_exec(t_commands *cmd, t_program_info *info)` | Apply redirs + exec (builtin or external) |

### Execution logic
```
execution(info):
  if my_commands is NULL or argv is NULL: return
  count = count_cmds(my_commands)
  if process_all_heredocs fails (Ctrl+C): return
  
  if count == 1 AND is_builtin(argv[0]):
    save stdin/stdout
    apply redirections
    exit_status = execute_builtin(cmd, info)
    restore stdin/stdout
  else:
    run_pipeline(info, count)  // handles both single external and multi-cmd
    
  free_commands(my_commands)
```

### Pipeline logic
```
run_pipeline(info, count):
  pids = malloc(count * sizeof(int))
  prev_fd = -1
  cmd = info->my_commands
  
  for each command i:
    if not last: pipe(pipe_fd)
    pids[i] = fork()
    if child:
      child_pipe_setup(prev_fd, pipe_fd, is_last)
      apply_redir(cmd)
      if is_builtin: execute_builtin(cmd, info); exit(status)
      else: execute_single_cmd(info)  // calls execve, never returns
    parent:
      close prev_fd if != -1
      close pipe_fd[1] (write end)
      prev_fd = pipe_fd[0] (read end for next cmd)
      cmd = cmd->next
  
  info->exit_status = wait_for_children(pids, count)
```

### Modify: `execution/execution_utils.c`
- Fix `is_builtin()`: return 1 for builtin, 0 for not (currently inverted)
- Remove `__attribute__((unused))`
- Add `void restore_stdio(t_program_info *info)` helper

---

## Phase 5: Signal Handling

### New file: `execution/signals.c`
| Function | Purpose |
|----------|---------|
| `void setup_interactive_signals(void)` | SIGINT: set g_signal + redisplay prompt. SIGQUIT: SIG_IGN |
| `void setup_exec_signals(void)` | Both SIGINT/SIGQUIT to SIG_DFL (called in child after fork) |
| `void setup_parent_exec_signals(void)` | SIGINT/SIGQUIT to SIG_IGN (parent ignores while children run) |
| `void setup_heredoc_signals(void)` | SIGINT: set g_signal to break read loop. SIGQUIT: SIG_IGN |

### Signal behavior summary
| Context | Ctrl+C (SIGINT) | Ctrl+\ (SIGQUIT) | Ctrl+D |
|---------|-----------------|-------------------|--------|
| Interactive prompt | New prompt, exit_status=130 | Nothing (ignored) | Exit shell |
| Running command | Kill child | Kill child + "Quit (core dumped)" | EOF to child stdin |
| Heredoc input | Cancel heredoc, new prompt | Nothing (ignored) | Warning about delimiter |

### Integration points
- `main.c`: Call `setup_interactive_signals()` at startup and after each command
- `execution.c`: Call `setup_parent_exec_signals()` before waiting, restore after
- `pipeline.c`: Call `setup_exec_signals()` in each child after fork
- `heredoc.c`: Call `setup_heredoc_signals()` before read loop
- After `waitpid`: check `WIFSIGNALED` -- if SIGINT print newline, if SIGQUIT print "Quit (core dumped)\n"

---

## Phase 6: Main Loop Restructuring

### Rewrite `read_from_prompt()` in `/home/liva/minishell/main.c`
```
read_from_prompt(info):
  while true:
    setup_interactive_signals()
    g_signal = 0
    line = readline("minishell$ ")
    if line == NULL:           // Ctrl+D
      ft_putstr_fd("exit\n", 1)
      break
    if g_signal != 0:
      info->exit_status = 130
      g_signal = 0
      free(line)
      continue
    if ft_strlen(line) > 0:
      add_history(line)
      info->my_commands = fill_command_struct(line, info->envp)  // use live env!
      if info->my_commands != NULL:
        execution(info)        // execution frees commands internally
    free(line)
```

Key changes from current code:
- Pass `info->envp` (not original `env`) so export/unset changes are visible
- Handle `readline()` returning NULL (Ctrl+D)
- Integrate signal checking after readline
- Remove `print_commands()` debug call

---

## Phase 7: Error Handling & Edge Cases

1. **Command not found vs permission denied**: Check `access(path, F_OK)` vs `access(path, X_OK)`. Exit 127 for not found, 126 for permission denied
2. **Relative paths with `/`**: If `argv[0]` contains `/`, try it directly without PATH search (e.g., `./script.sh`)
3. **Redirection errors must stop child**: If `open()` fails in `apply_redir`, child should `exit(1)` not continue to execve
4. **Empty command with redirections**: `> file` should create file but not execute anything -- check `argv[0] == NULL` after redirs
5. **PATH unset**: Only absolute/relative paths work, no PATH search

---

## New Files Summary

| File | Count | Purpose |
|------|-------|---------|
| `execution/env_utils.c` | 5 funcs | Environment array helpers |
| `execution/env_utils2.c` | 1-2 funcs | Identifier validation, PATH refresh |
| `execution/builtin_cd.c` | 1-2 funcs | cd command |
| `execution/builtin_export.c` | 4 funcs | export command |
| `execution/builtin_unset.c` | 1 func | unset command |
| `execution/builtin_exit.c` | 2 funcs | exit command |
| `execution/builtin_env.c` | 1 func | env command |
| `execution/heredoc.c` | 4 funcs | Heredoc processing |
| `execution/pipeline.c` | 4 funcs | Pipeline forking/waiting |
| `execution/signals.c` | 4 funcs | Signal handler setup |
| `execution/init_env.c` | 4 funcs | Moved from execution.c |

---

## Implementation Order

```
Phase 0  ──>  Phase 1  ──>  Phase 2  ──>  Phase 3  ──>  Phase 4  ──>  Phase 5  ──>  Phase 6  ──>  Phase 7
Headers       Env utils     Builtins      Heredoc       Pipes         Signals       Main loop     Edge cases
```

Each phase can be tested before moving to the next:
- **Phase 0-1**: Compiles, no behavior change
- **Phase 2**: Test builtins with single commands: `echo hello`, `pwd`, `cd /tmp`, `env`, `export A=1`, `unset A`, `exit`
- **Phase 3**: Test `cat << EOF`, `grep pattern << DELIM`
- **Phase 4**: Test `ls | cat`, `ls | grep a | wc -l`, `echo hello | cat > file`
- **Phase 5**: Test Ctrl+C at prompt, Ctrl+C during `cat`, Ctrl+\ during `cat`, Ctrl+D at prompt
- **Phase 6**: Full integration test
- **Phase 7**: Test `nonexistent_cmd`, `> file`, `/bin/ls`, `./minishell`

---

## Verification Checklist

- [ ] `echo hello world` prints "hello world\n"
- [ ] `echo -n hello` prints "hello" (no newline)
- [ ] `pwd` prints current directory
- [ ] `cd /tmp && pwd` prints "/tmp"
- [ ] `export FOO=bar && echo $FOO` prints "bar"
- [ ] `unset FOO && echo $FOO` prints empty line
- [ ] `env` prints all environment variables
- [ ] `exit` exits the shell
- [ ] `exit 42` exits with status 42
- [ ] `ls | grep minishell` works
- [ ] `cat << EOF` reads until EOF line
- [ ] `echo hello > file && cat file` prints "hello"
- [ ] `echo hello >> file` appends
- [ ] `cat < file` reads from file
- [ ] `$?` expands to last exit status
- [ ] Ctrl+C at prompt shows new prompt
- [ ] Ctrl+D at prompt exits
- [ ] Ctrl+\ at prompt does nothing
- [ ] No memory leaks in own code (test with `valgrind`)
