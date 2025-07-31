/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhapke <jhapke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 10:17:58 by jhapke            #+#    #+#             */
/*   Updated: 2025/07/31 11:55:42 by jhapke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Assignment name:    picoshell
Expected files:        picoshell.c
Allowed functions:    close, fork, wait, exit, execvp, dup2, pipe
___

Write the following function:

int    picoshell(char *cmds[]);

The goal of this function is to execute a pipeline. It must execute each
commands [sic] of cmds and connect the output of one to the input of the
next command (just like a shell).

Cmds contains a null-terminated list of valid commands. Each rows [sic]
of cmds are an argv array directly usable for a call to execvp. The first
arguments [sic] of each command is the command name or path and can be passed
directly as the first argument of execvp.

If any error occur [sic], The function must return 1 (you must of course
close all the open fds before). otherwise the function must wait all child
processes and return 0. You will find in this directory a file main.c which
contain [sic] something to help you test your function.


Examples: 
./picoshell /bin/ls "|" /usr/bin/grep picoshell
picoshell
./picoshell echo 'squalala' "|" cat "|" sed 's/a/b/g'
squblblb/
*/

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int	picoshell(char *cmds[])
{
	int		fd[2];
	int		i;
	pid_t	pid;
	int		prev_fd;
	int		j;

	j = -1;
	prev_fd = -1;
	i = -1;
	while (cmds[++i])
	{
		if (cmds[i + 1] != NULL)
		{
			if (pipe(fd) < 0)
				exit(1);
		}
		pid = fork();
		if (pid < 0)
		{
			close(fd[0]);
			close(fd[1]);
			exit(1);
		}
		if (pid == 0)
		{
			if (i != 0)
				dup2(prev_fd, STDIN_FILENO);
			if (cmds[i + 1] != NULL)
			{
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
			}
			close(fd[1]);
			execvp(&cmds[i][0], &cmds[i]);
			exit(1);
		}
		else
		{
			if (prev_fd != -1)
				close(prev_fd);
			prev_fd = fd[0];
			close(fd[1]);
		}
	}
	while (wait(NULL) > 0)
		;
	return (0);
}

/*
int	main(int ac, char **av)
{
	int	res;

	if (ac < 2)
	{
		printf("usage\n");
		return (1);
	}
	res = picoshell(av);
	return (res);
} */
