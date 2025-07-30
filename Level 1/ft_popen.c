/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhapke <jhapke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 14:33:55 by jhapke            #+#    #+#             */
/*   Updated: 2025/07/30 15:51:58 by jhapke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Allowed functions: pipe, fork, dup2, execvp, close, exit

write the following function:

    int    ft_popen(const char file, char const *argv[], char type)

The function must launch the executable file with the arguments argv (using execvp).
If the type is 'r' the function must return a file descriptor connected to the output of the command.
If the type is 'w' the function must return a file descriptor connected to the input of the command.
In case of error or invalid parameter the function must return -1.

example:

int main()
{
    int fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');

    char	line;
    while (line = get_next_line(fd))
        ft_putstr(line);
}

Hint: Do not leak file descriptors! */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int	ft_popen(const char file, char const **argv, char type)
{
	int		fd[2];
	pid_t	pid;

	if (!file || !argv || (type != 'r' && type != 'w'))
		return (-1);
	if (pipe(fd) < 0)
		exit(1);
	pid = fork();
	if (pid < 0)
		exit(1);
	if (pid == 0)
	{
		if (type == 'r')
		{
			close(fd[0]);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
		}
		else
		{
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
		}
		execvp(file, argv);
		exit(126);
	}
	else
	{
		if (type == 'w')
		{
			close(fd[0]);
			return (fd[1]);
		}
		close(fd[1]);
		return (fd[0]);
	}
}

int main()
{
	//int fd = open("texte", O_RDONLY);
	int fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');

	char buf[1];
	while(read(fd, buf, 1))
		write(1, buf, 1);
	close(fd);
	return (0);
}
