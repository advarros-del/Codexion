/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_struc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrvarga <adrvarga@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 09:38:36 by adrvarga          #+#    #+#             */
/*   Updated: 2026/07/27 09:38:50 by adrvarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	gettime(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

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
		g_info->coders[i].last_compile = 0;
		g_info->coders[i].status = 0;
		g_info->coders[i].thread = 0; //no se porque
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
	int j;

	i = 0;
	while (i < g_info->data->coders)
	{
		g_info->donglers[i].used = 0;
		g_info->donglers[i].last_release = 0;
		g_info->donglers[i].available = 0;
		g_info->donglers[i].requests = malloc(sizeof(t_heap_call) * g_info->data->coders);
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
}

int	fill_things(t_shared *g_info, t_coder *the_coders, t_dongle *donglers)
{
	fill_shared(g_info, the_coders, donglers);
	fill_coder(g_info);
	init_donglers(g_info);
	return (0);
}