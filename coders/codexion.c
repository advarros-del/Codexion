/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:04:55 by adrvarga          #+#    #+#             */
/*   Updated: 2026/11/22 21:06:05 by adrian           ###   ########.fr       */
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
	pthread_t	monitor;

	g_info.data = the_true_data(argv);
	g_info.coders = malloc(g_info.data->coders * sizeof(t_coder));
	if (!g_info.coders)
		return (in_the_end(&g_info), 1);
	g_info.donglers = malloc(g_info.data->coders * sizeof(t_dongle));
	if (!g_info.donglers || fill_things(&g_info, g_info.coders, g_info.donglers) == 1)
		return (in_the_end(&g_info), 1);
	if (pthread_create(&monitor, NULL, monitor_rutine, &g_info) != 0)
		return (0);
	create_pthreads(&g_info, g_info.coders);
	join_pthreads(&g_info, g_info.coders);
	pthread_join(monitor, NULL);
	in_the_end(&g_info);
	return (0);
}

