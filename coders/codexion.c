/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:04:55 by adrvarga          #+#    #+#             */
/*   Updated: 2026/11/21 17:58:31 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	the_timeline(t_shared *g_info)
{
	long long	t;

	pthread_mutex_lock(&g_info->print_mutex);
	t = gettime() - g_info->start_ime;
	pthread_mutex_unlock(&g_info->print_mutex);
	return (t);
}

void	create_pthreads(t_shared *g_info, t_coder *the_coders)
{
	int	i;

	i = 0;
	while (i < g_info->data->coders)
	{
		if (pthread_create(&the_coders[i].thread, NULL,
				coder_routine, &the_coders[i]) != 0)
			return ;
		i++;
	}
}

void	join_pthreads(t_shared *g_info, t_coder *the_coders)
{
	int	i;

	i = 0;
	while (i < g_info->data->coders)
	{
		pthread_join(the_coders[i].thread, NULL);
		i++;
	}
}

int	codexion(char **argv)
{
	t_shared	g_info;
	t_data		*data;
	t_coder		*the_coders;
	t_dongle	*donglers;
	pthread_t	monitor;

	data = the_true_data(argv);
	g_info.data = data;
	the_coders = malloc(data->coders * sizeof(t_coder));
	if (!the_coders)
		return (1);
	donglers = malloc(data->coders * sizeof(t_dongle));
	if (!donglers || fill_things(&g_info, the_coders, donglers) == 1)
		return (1);
	if (pthread_create(&monitor, NULL, monitor_rutine, &g_info) != 0)
		return (0);
	create_pthreads(&g_info, the_coders);
	join_pthreads(&g_info, the_coders);
	pthread_join(monitor, NULL);
	return (0);
}
