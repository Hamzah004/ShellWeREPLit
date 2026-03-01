/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 17:48:32 by hbani-at          #+#    #+#             */
/*   Updated: 2026/02/15 17:49:47 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int	main(void)
{
	pid_t	pid;
	char	*args[] = {"/bin/ls", "-la", NULL};

	pid = fork();
	if (pid == 0)
	{
		char *env[] = {NULL}; // Inherit parent's environment
		if (!access("/bin/ls", X_OK))
		{
			execve("/bin/ls", args, env);
			perror("execve failed");
		}
		else
		{
			printf("error\n");
		}
	}
	else if (pid > 0)
	{
		// Parent process
		wait(NULL); // Wait for child to finish
		printf("Child finished\n");
	}
	else
	{
		perror("fork failed");
	}
	return (0);
}
