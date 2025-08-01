/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johnhapke <johnhapke@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 10:17:58 by jhapke            #+#    #+#             */
/*   Updated: 2025/08/01 12:39:02 by johnhapke        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Assignment name:    picoshell
Expected files:        picoshell.c
Allowed functions:    close, fork, wait, exit, execvp, dup2, pipe
___

Write the following function:

int    picoshell(char **cmds[]);

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

/*int	main(void)
{
	int	res;

	printf("test1:");
	char *cmd1[] = {"ls", NULL};
	char *cmd2[] = {"grep", "picoshell", NULL};
	char **cmds1[] = {cmd1, cmd2, NULL};
	res = picoshell(cmds1);
	printf("\ntest2:");
	char *cmd3[] = {"ls", "-l", NULL};
	char **cmds2[] = {cmd3, NULL};
	res = picoshell(cmds2);
	printf("\ntest3:");
	char *cmd4[] = {"echo", "hello world", NULL};
	char *cmd5[] = {"cat", NULL};
	char *cmd6[] = {"wc", "-w", NULL};
	char **cmds3[] = {cmd4, cmd5, cmd6, NULL};
	res = picoshell(cmds3);
	printf("\ntest4:");
	char *cmd7[] = {"/bin/ls", NULL};
	char *cmd8[] = {"/usr/bin/grep", "picoshell", NULL};
	char **cmds4[] = {cmd7, cmd8, NULL};
	res = picoshell(cmds4);
	printf("\ntest5:");
	char *cmd9[] = {"echo", "test123", NULL};
	char *cmd10[] = {"cat", NULL};
	char **cmds5[] = {cmd9, cmd10, NULL};
	res = picoshell(cmds5);
	printf("\ntest6:");
	char *cmd11[] = {"ls", NULL};
	char *cmd12[] = {"wc", "-l", NULL};
	char **cmds6[] = {cmd11, cmd12, NULL};
	res = picoshell(cmds6);
	return (res);
}*/
