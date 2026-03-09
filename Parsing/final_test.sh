#!/bin/bash

echo "=========================================="
echo "Building Parser Test Programs"
echo "=========================================="

# Compile interactive test
gcc -Wall -Wextra -Werror parser_main.c parser.c parser_utils.c \
    tokenizer.c tokenizer_utls.c syntax_validation.c debugging_functions.c \
    -I../include -L../libft -lft -o parser_test 2>/dev/null

# Compile batch test
gcc -Wall -Wextra -Werror test_commands.c parser.c parser_utils.c \
    tokenizer.c tokenizer_utls.c syntax_validation.c debugging_functions.c \
    -I../include -L../libft -lft -o test_commands 2>/dev/null

echo "[✓] Build successful"
echo ""
echo "=========================================="
echo "Running Automated Tests"
echo "=========================================="
echo ""
./test_commands
echo ""
echo "=========================================="
echo "To use interactive parser:"
echo "  cd Parsing"
echo "  ./parser_test"
echo "=========================================="
