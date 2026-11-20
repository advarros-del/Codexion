/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rutines.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 15:01:33 by adrvarga          #+#    #+#             */
/*   Updated: 2026/11/20 15:02:23 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (1)
	{
		if (compiling(coder) == 1 || is_debugging(coder) == 1
			|| is_refactoring(coder) == 1)
			break ;
		if (is_sim_ended(coder->shared) == 1
			|| on_fire(coder->shared->coders) == 1)
		{
			pthread_mutex_lock(&coder->shared->end_mutex);
			coder->shared->simulation_end = 1;
			pthread_mutex_unlock(&coder->shared->end_mutex);
			break ;
		}
	}
	return (NULL);
}

/********************************************************
* -Check if some of the coders did all the compilations *
********************************************************/

int	is_sim_ended(t_shared *g_info)
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

/***************************************************
* -Check if any coder is burning                   *
***************************************************/

int	on_fire(t_coder *coders)
{
	int				i;
	long long int	now;

	now = gettime();
	i = -1;
	while (++i < coders->shared->data->coders)
	{
		pthread_mutex_lock(&coders[i].mutex);
		if ((now - coders[i].last_compile) >= coders->shared->data->burntime
			&& coders[i].status == 0)
		{
			pthread_mutex_lock(&coders[i].shared->end_mutex);
			coders[i].shared->simulation_end = 1;
			pthread_mutex_unlock(&coders[i].shared->end_mutex);
			printer(0, coders[i].id, coders->shared);
			coders[i].status = 1;
			pthread_mutex_unlock(&coders[i].mutex);
			return (1);
		}
		pthread_mutex_unlock(&coders[i].mutex);
	}
	return (0);
}

/***************************************************************************
* -Check if at the beginning of each routine some coder is done or burning *
***************************************************************************/

void	*monitor_rutine(void *arg)
{
	int			i;
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
				pthread_cond_broadcast(&g_info->donglers[i].cond);
				i++;
			}
			break ;
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
