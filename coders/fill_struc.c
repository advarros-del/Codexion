/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_struc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 09:38:36 by adrvarga          #+#    #+#             */
/*   Updated: 2026/11/21 20:14:00 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	fill_coder(t_shared *g_info)
{
	int	i;
	int	aux;

	i = 0;
	aux = g_info->data->coders;
	while (i < aux)
	{
		g_info->coders[i].id = i;
		g_info->coders[i].compile_counter = 0;
		g_info->coders[i].last_compile = g_info->start_ime;
		g_info->coders[i].status = 0;
		g_info->coders[i].ndong = 0;
		pthread_mutex_init(&g_info->coders[i].mutex, NULL);
		g_info->coders[i].thread = 0; //no se porque
		g_info->coders[i].shared = g_info;
		g_info->coders[i].r_don = &g_info->donglers[i];
		if (i == 0)
			g_info->coders[i].l_don = &g_info->donglers[aux - 1];
		else
			g_info->coders[i].l_don = &g_info->donglers[i -1];
		i++;
	}
}

int	init_donglers(t_shared *g_info)
{
	int	i;
	int	j;

	i = 0;
	while (i < g_info->data->coders)
	{
		g_info->donglers[i].used = 0;
		g_info->donglers[i].last_release = 0;
		g_info->donglers[i].available = 0;
		g_info->donglers[i].requests
			= malloc(sizeof(t_heap_call) * g_info->data->coders);
		j = 0;
		while (g_info->data->coders > j)
		{
			g_info->donglers[i].requests[j].id_code = j;
			g_info->donglers[i].requests[j].deadline = 0;
			g_info->donglers[i].requests[j].time_stamp = 0;
			j++;
		}
		pthread_cond_init(&g_info->donglers[i].cond, NULL);
		if (pthread_mutex_init(&g_info->donglers[i].mutex, NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

void	fill_shared(t_shared *g_info, t_coder *the_coders, t_dongle *donglers)
{
	g_info->coders = the_coders;
	g_info->donglers = donglers;
	g_info->simulation_end = 0;
	g_info->start_ime = gettime();
	pthread_mutex_init(&g_info->end_mutex, NULL);
	pthread_mutex_init(&g_info->print_mutex, NULL);
}

int	fill_things(t_shared *g_info, t_coder *the_coders, t_dongle *donglers)
{
	fill_shared(g_info, the_coders, donglers);
	fill_coder(g_info);
	init_donglers(g_info);
	return (0);
}

t_data	*the_true_data(char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->coders = ft_atol(argv[1]);
	data->burntime = ft_atol(argv[2]);
	data->compile_time = ft_atol(argv[3]);
	data->debug_time = ft_atol(argv[4]);
	data->refac_time = ft_atol(argv[5]);
	data->n_compile = ft_atol(argv[6]);
	data->dongle_cd = ft_atol(argv[7]);
	if (strlen(argv[8]) == 4)
		data->scheduler = 0;
	else
		data->scheduler = 1;
	return (data);
}
