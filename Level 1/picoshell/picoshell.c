/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johnhapke <johnhapke@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 10:17:58 by jhapke            #+#    #+#             */
/*   Updated: 2025/10/30 11:54:47 by johnhapke        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
				return (1);
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
			return (1);
		}
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				if (dup2(prev_fd, STDIN_FILENO) == -1)
						exit(1);
					close(prev_fd);
			}
			if (cmds[i + 1] != NULL)
			{
				close(fd[0]);
				if (dup2(fd[1], STDOUT_FILENO) == -1)
					exit(1);
				close(fd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		else
		{
			if (cmds[i + 1] != NULL)
			{
				if (prev_fd != -1)
					close(prev_fd);
				prev_fd = fd[0];
				close(fd[1]);
			}
			if (prev_fd != -1)
				close(prev_fd);
		}
	}
	int error = 0;
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) != 0)
			error = 1;
		}
		else if (WIFSIGNALED(status))
			error = 1;
	}
	return (error);
}
