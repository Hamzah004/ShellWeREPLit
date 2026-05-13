# Heredoc — remaining bugs

Reference baseline: `parsing/heredoc.c` (line numbers below are from that file).

---

## Bug 1 — Delimiter is compared AFTER expansion

**Where:** `parsing/heredoc.c:112-115`

```c
after_exp = heredoc_expansion(input, env, exit_status, quoted);
if (!after_exp)
    return (-1);
if (ft_strcmp(after_exp, drlimiter) == 0)
```

**What's wrong:** The user-typed line is expanded first, then compared to the delimiter.
Bash does the opposite: it matches the **raw** input line against the delimiter, and only
expands the line if it is NOT the terminator.

**Why it matters:**

```bash
export EOF=foo
cat << foo
$EOF        # expands to "foo" → with current code, ENDS the heredoc immediately
foo         # raw "foo" → should end the heredoc here
```

Your current code: `$EOF` expands to `foo`, matches delimiter `foo`, heredoc closes early.
Correct behaviour: `$EOF` is raw text, doesn't match, gets expanded and written as body.

**Fix sketch:** compare `input` to `drlimiter` first; only call `heredoc_expansion`
if it's not the terminator.

---

## Bug 2 — Quotes in the delimiter are not stripped

**Where:** the delimiter string comes from the tokenizer through
`parsing/parser.c` → `new_redirection` (`parsing/parser_utils.c:40`).
The tokenizer keeps quote characters in token values (see `handle_word` in
`parsing/tokenizer.c:73`), and `yokotenkai` (`parsing/fill_command.c:53-63`)
explicitly **skips** `TOK_HEREDOC` for `replace_str`, so heredoc delimiters
never get their quotes stripped.

**What's wrong:**

```bash
cat << "EOF"
```

Stored delimiter is the literal 5-character string `"EOF"` (with quotes).
The user types `EOF` (3 chars) to end → `ft_strcmp` never matches → heredoc never terminates.

**Fix sketch:** before storing the delimiter on the `t_redir` node (or before passing it
to `handel_herdoc`), strip outer quotes. Also see Bug 3 — you need to remember
**that** it was quoted, separately from the stripped value.

---

## Bug 3 — No "delimiter was quoted" flag tracked anywhere

**Where:** `struct s_redir` in `include/parsing.h:41-47` has no field for this.
`collect_heredocs` (`parsing/heredoc.c:87`) hardcodes `quoted = 1`.

**What's wrong:** Bash rule: if the delimiter is quoted (`<< "EOF"` or `<< 'EOF'`),
variable expansion inside the body is **disabled**. Currently you have no way to
know whether the delimiter was quoted, so you always expand.

**Fix sketch:**
1. Add `int expand_body;` (or similar) to `t_redir`.
2. When building the heredoc redir node, detect if the delimiter token had any
   quote character; set the flag accordingly.
3. Pass that flag into `handel_herdoc` instead of the hardcoded `1`.

---

## Bug 4 — Wrong expander for heredoc body (`expand_argv`)

**Where:** `parsing/heredoc.c:50` (inside `heredoc_expansion`)

```c
tmp = expand_argv(input, env, exit_status);
```

**What's wrong:** `expand_argv` is built for command arguments — it does **word splitting**
(splits the result on whitespace). Heredocs in bash do **NOT** do word splitting:
variables are substituted **in place**, preserving the line exactly.

Example: if `HOME=/home/foo`, the body line

```
hello $HOME world
```

should produce exactly `hello /home/foo world` (one line). With `expand_argv` doing
word splitting and `handel_multyarg` re-joining **without a separator**
(`parsing/heredoc.c:29` — `ft_strjoin(output, tmp[i])`), you can get glued-together
output like `hello/home/fooworld`.

**Fix sketch:** write a heredoc-specific expander. Walk the input char-by-char,
copy normal chars, when you hit `$` look up the variable (reuse `get_env_value`,
`$?`, etc.), append the value. No splitting, no rejoining.

---

## Bug 5 — Crash if `tmp[0]` is NULL

**Where:** `parsing/heredoc.c:62`

```c
final_output = ft_strdup(tmp[0]);
```

**What's wrong:** if `expand_argv` returns an array where `tmp[0]` is `NULL` (e.g.
the input expanded to nothing), `ft_strdup(NULL)` is undefined behaviour and
usually segfaults.

**Fix sketch:** guard the case — `if (!tmp[0]) final_output = ft_strdup("");`.
Or: this whole branch disappears once Bug 4 is fixed (no more `expand_argv` here).

---

## Bug 6 — fd leak on early returns in `handel_herdoc`

**Where:** `parsing/heredoc.c:110-114`

```c
input = readline("> ");
if (!input)
    return (-1);            // ← fd[0] and fd[1] still open
after_exp = heredoc_expansion(input, env, exit_status, quoted);
if (!after_exp)
    return (-1);            // ← fd[0] and fd[1] still open
```

**What's wrong:** the pipe is opened at the top of the function but both ends are
**not closed** on these failure paths. Every Ctrl-D or expansion failure during
a heredoc leaks two file descriptors.

**Fix sketch:** before each `return (-1)`, do `close(fd[0]); close(fd[1]);`.

Bonus: bash also prints a warning when a heredoc is terminated by EOF instead of
the delimiter:

```
bash: warning: here-document at line N delimited by end-of-file (wanted `EOF')
```

Optional but nice.

---

## Bug 7 — `quoted` parameter name is inverted

**Where:** `parsing/heredoc.c:39-49` (and the call site at line 87)

```c
char *heredoc_expansion(char *input, t_envp *env, int exit_status, int quoted)
{
    if (!quoted)
        final_output = ft_strdup(input);   // no expansion
    ...
    tmp = expand_argv(input, env, exit_status);   // expansion
```

**What's wrong:** when `quoted == 1` the function **expands**; when `quoted == 0`
it doesn't. The name implies the opposite (a "quoted" delimiter should mean
"do NOT expand"). This is a footgun every time a future reader looks at it.

**Fix sketch:** rename to `expand` / `do_expand`, or flip the meaning and update
all call sites. Do it as part of Bug 3's wiring.

---

## Suggested fix order

1. **Bug 1** (compare before expand) — small, observable, makes heredoc semantics correct.
2. **Bug 6** (close fds on early return) — small defensive cleanup.
3. **Bug 2 + Bug 3 + Bug 7** (quoted delimiter + flag + naming) — do them together;
   they share the same code paths.
4. **Bug 4** (write the right expander) — biggest change; once done, Bug 5 disappears.
5. **Bug 5** (NULL guard) — only needed until Bug 4 is in.
