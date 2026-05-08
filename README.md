a custom, lightweight Unix shell interpreter developed in C, designed to emulate core bash functionality. This project covers essential command-line interface concepts, including prompt display, command execution, parsing, signal handling, and pipeline/redirection management. It serves as a comprehensive introduction to process management and low-level system calls.

## Resources

- [bash reference](https://www.gnu.org/software/bash/manual/bash.html)
- [shell wiki](https://en.wikipedia.org/wiki/Shell_(computing))
- [shell builtin](https://en.wikipedia.org/wiki/Shell_builtin)
- [process wiki](https://en.wikipedia.org/wiki/Process_(computing))
- [BEHIND THE SCENES: What happens when you execute a command in the shell?](https://medium.com/@SergioPietri/behind-the-scenes-what-happens-when-you-execute-a-command-in-the-shell-9fef53832f6a)
- [Medium Building a mini-bash](https://m4nnb3ll.medium.com/minishell-building-a-mini-bash-a-42-project-b55a10598218)
- [Building a Parser from scratch](https://www.youtube.com/watch?v=4m7ubrdbWQU&t=38s)
- [Heredoc: A Deep Dive](https://medium.com/@oduwoledare/heredoc-a-deep-dive-23c82992e522)
- [How to distinguish between builtin and external util?](https://unix.stackexchange.com/questions/371722/how-to-distinguish-between-builtin-and-external-util-e-g-echo)
- [42-minishell explained github](https://github.com/multitudes/42-minishell?tab=readme-ov-file)
- [dup() and dup2() Linux system call](https://www.geeksforgeeks.org/c/dup-dup2-linux-system-call/)
- [ctrl d is line enter](https://hackarcana.com/article/ctrl-d-is-like-enter)
- [What does Ctrl D hmean on Unix Linux systems](https://www.quora.com/What-does-Ctrl-D-mean-on-Unix-Linux-systems)
- valgrind test: valgrind --leak-check=full --show-leak-kinds=all --suppressions=supp.supp --trace-children=yes --track-fds=yes ./minishell
