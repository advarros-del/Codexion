/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrvarga <adrvarga@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:04:55 by adrvarga          #+#    #+#             */
/*   Updated: 2026/06/23 17:04:55 by adrvarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	the_true_parser(char **argv, t_data **data)
{
	(*data)->coders = ft_atol(argv[1]);
	(*data)->burntime = ft_atol(argv[2]);
	(*data)->compile_time = ft_atol(argv[3]);
	(*data)->debug_time = ft_atol(argv[4]);
	(*data)->refac_time = ft_atol(argv[5]);
	(*data)->n_compile = ft_atol(argv[6]);
	(*data)->dongle_cd = ft_atol(argv[7]);
	if (strlen(argv[8]) == 4)
		(*data)->scheduler = 0;
	else
		(*data)->scheduler = 1;
}

int	codexion(char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	the_true_parser(argv, &data);
	return (0);
}
