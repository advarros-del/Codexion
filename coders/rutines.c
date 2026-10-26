/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rutines.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 15:01:33 by adrvarga          #+#    #+#             */
/*   Updated: 2026/10/25 19:30:17 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void	is_debugging(t_coder *coder)
{
	printer(5, coder->id, coder->shared);
	usleep(coder->shared->data->debug_time * 1000);
}

void	is_refactoring(t_coder *coder)
{
	printer(6, coder->id, coder->shared);
	usleep(coder->shared->data->refac_time * 1000);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (1)
	{
		if (compiling(coder) == 1)
			break;
		is_debugging(coder);
		is_refactoring(coder);
		if (is_sim_ended(coder->shared) == 1 || on_fire(coder->shared->coders) == 1)
			break;		
	}
	return (NULL);
}


int	is_sim_ended(t_shared* g_info)
{
	int	i;

	i = 0;
	while (i < g_info->data->coders)
	{
		pthread_mutex_lock(&g_info->coders[i].mutex);
		if (g_info->coders[i].compile_counter < g_info->data->n_compile)
		{
			pthread_mutex_unlock(&g_info->coders[i].mutex);
			return (0);
		}
		pthread_mutex_unlock(&g_info->coders[i].mutex);
		i++;
	}
	return (1);
}

int	on_fire(t_coder *coders)
{
	int	i;
	long long	int now;

	now = gettime();
	i = 0;
	while (i < coders->shared->data->coders)
	{
		pthread_mutex_lock(&coders[i].mutex);
		if ((now - coders[i].last_compile) >= coders->shared->data->burntime)
		{	
			printer(1, coders->id, coders->shared);
			pthread_mutex_unlock(&coders[i].mutex);
			return (1);
		}
		pthread_mutex_unlock(&coders[i].mutex);
		i++;
	}
	return (0);
}

void	*monitor_rutine(void *arg)
{
	int	i;
	t_shared	*g_info;

	g_info = (t_shared *)arg;

	i = 0;
	while (1)
	{
		if (is_sim_ended(g_info) == 1 || on_fire(g_info->coders) == 1)
			{
				pthread_mutex_lock(&g_info->end_mutex);
				g_info->simulation_end = 1;
				pthread_mutex_unlock(&g_info->end_mutex);
				while (g_info->data->coders > i)
				{
					pthread_cond_signal(&g_info->donglers[i].cond);
					i++;
				}
				break;
			}
		usleep(100);
	}
	return (NULL);
}


int	is_sim(t_shared *g_info)
{
	int	i;

	pthread_mutex_lock(&g_info->end_mutex);
	i = g_info->simulation_end;
	pthread_mutex_unlock(&g_info->end_mutex);
	return (i);
}

