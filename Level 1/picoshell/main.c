/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhapke <jhapke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:33:51 by jhapke            #+#    #+#             */
/*   Updated: 2025/08/03 15:38:22 by jhapke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int	picoshell(char **cmds[]);

int	main(int ac, char **av)
{
	char	***cmds;
	int		i;
	int		count_array;
	int		count_cmds;
	int		start;
	int		result;

	i = 1;
	count_array = 0;
	if (ac < 2)
		return (1);
	cmds = malloc(100 * sizeof(char **));
	if (!cmds)
		return (1);
	while (av[i])
	{
		start = i;
		while (av[i + 1] && strcmp(av[i], "|") != 0)
			i++;
		cmds[count_array] = malloc((i - count_cmds) * sizeof(char *));
		if(!cmds[count_array])
			return (1);
		cmds[count_array][i - count_cmds] = NULL;
		count_cmds = 0;
		while (start <= i)
			cmds[count_array][count_cmds++] = av[start++];
		count_array++;
		i++;
	}
	result = picoshell(cmds);
	i = 0;
	while (i < count_array)
		free(cmds[i++]);
	free(cmds);
	return (result);
}
