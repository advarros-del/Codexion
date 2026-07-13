/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrvarga <adrvarga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:04:55 by adrvarga          #+#    #+#             */
/*   Updated: 2026/07/13 23:14:16 by adrvarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


static void	the_true_parser(char **argv, t_data **data);

//  int get_time_ms()
// {
// 	int n;
// 	n = gettimeofday();
// 	n_ms = n * 100
// 	return (n)
// }
void	the_true_data(char **argv, t_data **data)
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
	t_shared g_info;
	t_data	*data;
	pthread_t *threads;
	t_coder	*the_coders;
	t_dongle *donglers;

	data = malloc(sizeof(t_data));
	the_true_data(argv, &data);
	g_info.data = data;
	threads = malloc(data->coders * sizeof(pthread_t));
	the_coders = malloc(data->coders * sizeof(t_coder));
	g_info.coders = the_coders;
	donglers = malloc(data->coders * sizeof(t_dongle));
	g_info.donglers = donglers;
	g_info.simulation_end = 1;
	g_info.start_ime = gettimeofday();
	
	return (0);
} 

pthread_t	fill_coder(pthread_t treads,t_data data)
{
	return treads;
	
}

long get_time_ms()
{
	int	tm;
	
	tm = gettimeofday();
	
	
}


