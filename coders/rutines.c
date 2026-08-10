/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rutines.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrvarga <adrvarga@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 15:01:33 by adrvarga          #+#    #+#             */
/*   Updated: 2026/07/27 15:01:33 by adrvarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compiling(t_coder *coder)
{
	if (take_dong(coder->l_don, coder) == 1)
		return;
	printf("the coder %d take a dongler", coder->id);
	if (take_dong(coder->r_don, coder) == 1)
	{
		pthread_mutex_lock(&coder->l_don->mutex);
		coder->l_don->available = 0;
		pthread_mutex_unlock(&coder->l_don->mutex);
		return;
	}
	printf("the coder %d take a dongler", coder->id);
	printf("coder %d compiling", coder->id);
	pthread_mutex_lock(&coder->mutex);
	coder->compile_counter += 1;
	pthread_mutex_unlock(&coder->mutex);
	coder->last_compile = gettime();
	sleepnow(coder->shared->data->compile_time);
	pthread_mutex_lock(&coder->l_don->mutex);
	coder->l_don->available = 0;
	pthread_mutex_unlock(&coder->l_don->mutex);
	pthread_cond_signal(&coder->l_don->cond);
	pthread_mutex_lock(&coder->r_don->mutex);
	coder->r_don->available = 0;
	pthread_mutex_unlock(&coder->r_don->mutex);
	pthread_cond_signal(&coder->r_don->cond);
}

void	is_debugging(t_coder *coder)
{
	printf("coder %d is debugging", coder->id);
	sleepnow(coder->shared->data->debug_time);
}

void	is_refactoring(t_coder *coder)
{
	printf("coder %d is refactoring", coder->id);
	sleepnow(coder->shared->data->refac_time);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (1)
	{
		compiling(coder);
		is_debugging(coder);
		is_refactoring(coder);
		if (is_sim_ended(coder->shared) == 1 || on_fire(coder) == 1)
			break;		
	}
	return (NULL);
}

int	take_dong(t_dongle *x_don, t_coder *x_coder)
{
	pthread_mutex_lock(&x_don->mutex);
	while (1)
	{
		if (x_don->available == 0)
			{
				x_don->available = 1;
				break;
			}
		else
			{
				pthread_cond_wait(&x_don->cond, &x_don->mutex);
				if (x_coder->shared->simulation_end == 1)
				{
					pthread_mutex_unlock(&x_don->mutex);
					return (1);
				}
			}
	}
	pthread_mutex_unlock(&x_don->mutex);
	return (0);
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
			printf("Coder %d is burned", coders[i].id);
			pthread_mutex_unlock(&coders[i].mutex);
			return (1);
		}
		pthread_mutex_unlock(&coders[i].mutex);
		i++;
	}
	return (0);
}

int	monitor_rutine(t_shared *g_info)
{
	int	i;

	i = 0;
	while (1)
	{
		if (is_sim_ended(g_info) == 1 || on_fire(g_info->coders) == 1)
			{
				while (g_info->data->coders > i)
				{
					pthread_cond_signal(&g_info->donglers[i].cond);
					i++;
				}
				break;
			}
		sleepnow(100);
	}
		return (1);
}