/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrvarga <adrvarga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 14:56:11 by adrvarga          #+#    #+#             */
/*   Updated: 2026/08/10 20:30:03 by adrvarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_digit(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] != '\0')
	{
		if (!(arg[i] >= '0' && arg[i] <= '9'))
			return (1);
		i++;
	}
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
			break;
		i++;
	}
	if (i >= 3)
		return (0);
	i = 0;
	while (arg[i] != '\0')
	{
		if (arg[i] != opt2[i])
			break;
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
	while (i > 8)
	{
		if (is_digit(argv[i]) == 1)
		{
			printf("The argument %d is incorrect.", i);
			return (1);
		}
		if (strlen(argv[i]) > 4 || is_valid(argv[i]) == 1)
		{
			printf("The argument %d is incorrect.", i);
			return (1);
		}
		i++;
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
	if (parser_error(argv) == 1 || codexion(argv) == 1) // comprobar el limite  de int
	{
		printf("Something goes wrong");
		return (1);
	}
	return (0);
}

