/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhapke <jhapke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 10:17:58 by jhapke            #+#    #+#             */
/*   Updated: 2025/08/03 14:33:34 by jhapke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Assignment name:    picoshell
Expected files:        picoshell.c
Allowed functions:    close, fork, wait, exit, execvp, dup2, pipe
___

Write the following function:

int    picoshell(char **cmds[]);

The goal of this function is to execute a pipeline. It must execute each
commands of cmds and connect the output of one to the input of the
next command (just like a shell).

Cmds contains a null-terminated list of valid commands. Each rows
of cmds are an argv array directly usable for a call to execvp. The first
arguments of each command is the command name or path and can be passed
directly as the first argument of execvp.

If any error occur, The function must return 1 (you must of course
close all the open fds before). otherwise the function must wait all child
processes and return 0. You will find in this directory a file main.c which
contain something to help you test your function.


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
#include <string.h>

int	picoshell(char **cmds[])
{
	int		fd[2];
	int		i;
	pid_t	pid;
	int		prev_fd;
	int		status;

	i = -1;
	prev_fd = -1;
	fd[0] = -1;
	fd[1] = -1;
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
			if (cmds[i + 1] != NULL)
			{
				close(fd[0]);
				close(fd[1]);
				if (prev_fd != -1)
					close(prev_fd);
			}
			exit(1);
		}
		if (pid == 0)
		{
			if (i != 0)
				if (dup2(prev_fd, STDIN_FILENO) == -1)
				{
					if (cmds[i + 1] != NULL)
					{
						close(fd[0]);
						close(fd[1]);
						if (prev_fd != -1)
							close(prev_fd);
					}
					exit(1);
				}
			if (cmds[i + 1] != NULL)
			{
				if (dup2(fd[1], STDOUT_FILENO) == -1)
				{
					close(fd[0]);
					close(fd[1]);
					if (prev_fd != -1)
						close(prev_fd);
					exit(1);
				}
				close(fd[0]);
			}
			if (fd[1] != -1)
				close(fd[1]);
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		else
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (cmds[i + 1] != NULL)
			{
				prev_fd = fd[0];
				close(fd[1]);
			}
		}
	}
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) != 0)
			return (1);
		}
		else
			return (1);
	}
	return (0);
}
