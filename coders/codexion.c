/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrvarga <adrvarga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:04:55 by adrvarga          #+#    #+#             */
/*   Updated: 2026/09/14 22:14:31 by adrvarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
	t_shared	g_info;
	t_data		*data;
	t_coder		*the_coders;
	t_dongle	*donglers;
	pthread_t	monitor;
	int	i;

	data = malloc(sizeof(t_data));
	the_true_data(argv, &data);
	g_info.data = data;
	the_coders = malloc(data->coders * sizeof(t_coder));
	donglers = malloc(data->coders * sizeof(t_dongle));
	if (fill_things(&g_info, the_coders, donglers) != 0)
		return (1);
	if (pthread_create(&monitor, NULL, monitor_rutine, &g_info) != 0)
			return (0);
	i = 0;
	while (i < g_info.data->coders)
	{
		if (pthread_create(&g_info.coders[i].thread, NULL,
				coder_routine, &g_info.coders[i]) != 0)
			return (0);
		i++;
	}
	i = 0;
	while (i < g_info.data->coders)
	{
		pthread_join(g_info.coders[i].thread, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	return (0);
}

void	printer(int phrase, int coder, t_shared g_info)
{
	char		list[][];
	long long	timeline;
	
	timeline = the_timeline(g_info);
	list[4][16] = {
		"is burned",
		"take a dongler",
	 	"leave a dongler",
	 	"is compiling",
	 	"is debugging",
	 	"is refactoring"}
	printf("%d Coder %d %s \n", timeline, coder, list[phrase])
}