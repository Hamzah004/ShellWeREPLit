# Heredoc Problem: Partially-Quoted Delimiters (`<< 1''`)

## Test

```
$ cat << 1''
BODY=[$USER]
1
```

### Bash (reference)
```
BODY=[$USER]
```
- Delimiter dequotes to `1` → the heredoc ends when you type `1`.
- Because the delimiter contained quotes, the body is **not** expanded → `$USER` stays literal.

### Minishell (current behavior)
```
minishell$ cat << 1''
> BODY=[$USER]
> 1''
> minishell$ exit
```
- The heredoc **cannot be terminated** by any typed line (neither `1` nor `1''`). It only stops on EOF / Ctrl-D, and `cat` never runs.
- It would also **expand** the body (`$USER` → `liva`), which is the opposite of bash.

> Note: the `> ` prefixes above are readline's heredoc prompt echoed onto stdout when input is piped; they are not part of `cat`'s output.

---

## Root Cause

Two separate problems combine here.

### 1. `is_quoted` only checks the first and last character

`parsing/heredoc_utils.c`:
```c
if (len >= 2
    && ((delimiter[0] == '\'' && delimiter[len - 1] == '\'')
        || (delimiter[0] == '"' && delimiter[len - 1] == '"')))
    return (1);
```

`1''` starts with `1`, so `is_quoted` returns `0` → the delimiter is treated as fully
unquoted → `quoted = 0` → expansion is turned ON.

Bash's rule is different: a delimiter counts as "quoted" (expansion OFF) if it contains
**any** quote character *anywhere*, not just at both ends.

### 2. The terminator check compares the *expanded* line to the *raw* delimiter

`parsing/heredoc.c`:
```c
after_exp = heredoc_expansion(input, env, exit_status, quoted); // quoted=0 → expand → strips quotes
if (ft_strcmp(after_exp, drlimiter) == 0)                       // drlimiter is still "1''"
```

Because `quoted == 0`, every typed line is run through `expand_argv`, which **removes
quotes**. So a typed line `1''` becomes `1` after expansion, but `drlimiter` is still
`1''`. They can never be equal → the loop only ends at EOF.

This is why the previously fixed cases still work but this one does not:
- `<< "EOF"` → `is_quoted` = 1 → `replace_str` strips it to `EOF` **and** `quoted = 1`
  disables expansion → raw line `EOF` matches `EOF`. OK.
- `<< EOF` → delimiter `EOF`, no quotes/vars, expansion is a no-op → `EOF` matches `EOF`. OK.
- `<< 1''` → slips through both mechanisms. BROKEN.

---

## How to Solve

Adopt bash's model: **separate termination from body expansion.**

1. **`quoted` = "delimiter contains any quote char"** — rewrite `is_quoted` to scan the
   whole string for `'` or `"`, instead of only comparing the first/last character.

2. **Always dequote the delimiter once** to get the match target
   (`1''` → `1`, `"EOF"` → `EOF`). `expand_files` / `replace_str`
   (`parsing/command_expansion_utils*.c`) already strips all quotes, so call it for the
   delimiter **regardless** of `quoted`, not only when `quoted` is set.
   - Remember `replace_str` frees its argument and `r->file` is owned by the redirection
     struct, so pass an `ft_strdup(r->file)` copy (as `run_heredoc` already does).

3. **Compare the *raw* input line to the dequoted delimiter**, and expand the body
   *separately* (only when `!quoted`). Right now termination and expansion share one
   `after_exp` string, which is what entangles them. The loop should be roughly:
   ```
   read raw line
   if (ft_strcmp(raw_line, dequoted_delim) == 0) -> stop
   line_to_write = quoted ? raw_line : expand(raw_line)
   write line_to_write to pipe
   ```

### Files this touches
- `parsing/heredoc_utils.c` — `is_quoted` (scan for any quote).
- `parsing/heredoc.c` — `run_heredoc` (always dequote), and the compare/expand order
  inside `handel_herdoc`.

### Verification
```
# terminates on 1, body NOT expanded
printf 'cat << 1'"''"'\nBODY=[$USER]\n1\n' | ./minishell    # -> BODY=[$USER]

# regression: still works
printf 'cat << EOF\nx $USER\nEOF\n'        | ./minishell    # -> x <username>
printf 'cat << "EOF"\nx $USER\nEOF\n'      | ./minishell    # -> x $USER
```
Compare each against plain `bash`.
