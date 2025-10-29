/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: johnhapke <johnhapke@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 14:33:55 by jhapke            #+#    #+#             */
/*   Updated: 2025/10/29 06:47:39 by johnhapke        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int	ft_popen(const char *file, char *const *argv, char type)
{
	int		fd[2];
	pid_t	pid;

	if (!file || !argv || (type != 'r' && type != 'w'))
		return (-1);
	if (pipe(fd) < 0)
		exit(1);
	pid = fork();
	if (pid < 0)
	{
		close(fd[0]);
		close(fd[1]);
		exit(1);
	}
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
/*
use cases for read:
- ./a.out read echo hello
- ./a.out read ls
- ./a.out read cat /etc/passwd
- ./a.out read nonexistent_cmd -> correct behaviour -> return read: 0

use cases for read:
- ./a.out write cat hello
- ./a.out write wc -w
- ./a.out grep "z"
*/
int	main(int ac, char *const *av)
{
	char	buf[1];
	int		bytes;
	int		fd;
	char	c;

	if (ac < 2)
	{
		printf("usage: ./program [read/write] [command] [whatever] ...");
		return (1);
	}
	if (strncmp("read", av[1], 4) == 0)
		c = 'r';
	else if (strncmp("write", av[1], 5) == 0)
		c = 'w';
	else
		c = 'a';
	fd = ft_popen((const char *)av[2], av + 2, c);
	printf("fd = %i\n", fd);
	if (c == 'r')
	{
		while (1)
		{
			bytes = read(fd, buf, 1);
			printf("read returns: %d\n", bytes);
			if (bytes <= 0)
				break ;
			write(1, buf, 1);
		}
	}
	else if (c == 'w')
		write(fd, "Hallo Test\n", 11);
	close(fd);
	return (0);
}
