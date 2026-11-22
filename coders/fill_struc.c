/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_struc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 09:38:36 by adrvarga          #+#    #+#             */
/*   Updated: 2026/11/22 21:38:44 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	fill_coder(t_shared *g_info)
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
		g_info->coders[i].thread = 0; //no se porque
		g_info->coders[i].shared = g_info;
		g_info->coders[i].r_don = &g_info->donglers[i];
		if (i == 0)
			g_info->coders[i].l_don = &g_info->donglers[aux - 1];
		else
			g_info->coders[i].l_don = &g_info->donglers[i -1];
		i++;
	}
	return (i);
}


static void fill_dongles(t_shared *g_info, t_dongle *donglers)
{
	int	i;
	int	j;

	i = -1;
	while (++i < g_info->data->coders)
	{
		donglers[i].used = 0;
		donglers[i].last_release = 0;
		donglers[i].available = 0;
		j = -1;
		while (g_info->data->coders > ++j)
		{
			donglers[i].requests[j].id_code = j;
			donglers[i].requests[j].deadline = 0;
			donglers[i].requests[j].time_stamp = 0;
		}
	}
}

static int	fill_shared(t_shared *g_info, t_coder *the_coders, t_dongle *donglers)
{
	g_info->coders = the_coders;
	g_info->donglers = donglers;
	g_info->simulation_end = 0;
	g_info->start_ime = gettime();
	if (pthread_mutex_init(&g_info->end_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&g_info->print_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&g_info->end_mutex);
		return(1);
	}
	return (0);
}

int	fill_things(t_shared *g_info, t_coder *the_coders, t_dongle *donglers)
{
	int i;

	if (fill_shared(g_info, the_coders, donglers) != 0)
		return (1);
	if (init_coder(g_info) == -1)
		return (clean_shared(g_info), 1);
	fill_coder(g_info);
	i = init_donglers(donglers, g_info->data->coders);
	if (i != g_info->data->coders)
	{
		clean_donglers(donglers, i);
		clean_coders(the_coders, g_info->data->coders);
		clean_shared(g_info);
		return (1);
	}
	fill_dongles(g_info, donglers);
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
