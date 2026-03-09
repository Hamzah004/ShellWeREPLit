# Parser Testing Guide

This document describes how to test the parser module.

## Quick Start

### Interactive Parser Test
```bash
cd Parsing
gcc -Wall -Wextra -Werror parser_main.c parser.c parser_utils.c \
    tokenizer.c tokenizer_utls.c syntax_validation.c debugging_functions.c \
    -I../include -L../libft -lft -o parser_test
./parser_test
```

Then type commands to test:
```
$ echo "hello world"
$ ls -la | grep test
$ cat < input.txt
$ echo output > file.txt
$ exit
```

### Batch Testing
```bash
cd Parsing
gcc -Wall -Wextra -Werror test_commands.c parser.c parser_utils.c \
    tokenizer.c tokenizer_utls.c syntax_validation.c debugging_functions.c \
    -I../include -L../libft -lft -o test_commands
./test_commands
```

## Features Tested

### Tokenization
- Basic word parsing
- Double quoted strings (quotes are removed)
- Single quoted strings (quotes are removed)
- Operators: `|`, `<`, `>`, `>>`, `<<`

### Commands
- Simple commands: `echo hello`
- Commands with pipes: `ls -la | grep test`
- Commands with input redirection: `cat < input.txt`
- Commands with output redirection: `echo test > output.txt`
- Commands with append: `ls >> results.txt`
- Commands with heredoc: `cat << EOF`

### Syntax Validation
The parser validates:
- Pipes must have commands on both sides
- Redirections must be followed by a filename
- No consecutive operators

## Known Limitations

- Heredoc content is not processed (only the delimiter is captured)
- Environment variable expansion is not implemented
- Command substitution is not implemented
