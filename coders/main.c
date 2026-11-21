/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 14:56:11 by adrvarga          #+#    #+#             */
/*   Updated: 2026/11/21 20:10:31 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_digit(char *arg)
{
	int			i;
	long int	aux;

	i = 0;
	while (arg[i] != '\0')
	{
		if (!(arg[i] >= '0' && arg[i] <= '9'))
			return (1);
		i++;
	}
	aux = 2147483647;
	if (ft_atol(arg) > aux)
		return (1);
	return (0);
}

static int	is_valid(char *arg)
{
	int		i;
	char	*opt1;
	char	*opt2;

	i = 0;
	opt1 = "fifo";
	opt2 = "edf";
	while (arg[i] != '\0')
	{
		if (arg[i] != opt1[i])
			break ;
		i++;
	}
	if (i >= 3)
		return (0);
	i = 0;
	while (arg[i] != '\0')
	{
		if (arg[i] != opt2[i])
			break ;
		i++;
	}
	if (i >= 2)
		return (0);
	return (1);
}

static int	parser_error(char **argv)
{
	int	i;

	i = 1;
	while (i <= 7)
	{
		if (is_digit(argv[i]) == 1)
		{
			printf("The argument %d is incorrect.", i);
			return (1);
		}
		i++;
	}
	if (strlen(argv[i]) > 4 || is_valid(argv[i]) == 1)
	{
		printf("The argument %d is incorrect.", i);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	if (argc != 9)
	{
		printf("Incorrect number of arguments.Please read README.md");
		printf(" for correct usage");
		return (1);
	}
	if (parser_error(argv) == 1 || codexion(argv) == 1)
	{
		printf("Something goes wrong");
		return (1);
	}
	return (0);
}

