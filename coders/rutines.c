/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rutines.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrvarga <adrvarga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 15:01:33 by adrvarga          #+#    #+#             */
/*   Updated: 2026/09/21 21:04:54 by adrvarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	compiling(t_coder *coder)
{
	if (take_dong(coder->l_don, coder) == 1)
		return (1);
	printer(2, coder->id, coder->shared);
	if (take_dong(coder->r_don, coder) == 1)
	{
		pthread_cond_signal(&coder->r_don->cond);
		is_available(coder->l_don);
		return(1);
	}
	printer(2, coder->id, coder->shared);	
	printer(4, coder->id, coder->shared);
	pthread_mutex_lock(&coder->mutex);
	coder->compile_counter += 1;
	coder->last_compile = gettime();
	pthread_mutex_unlock(&coder->mutex);
	usleep(coder->shared->data->compile_time * 1000);
	is_available(coder->l_don);
	printer(3, coder->id, coder->shared);
	pthread_cond_signal(&coder->l_don->cond);
	is_available(coder->r_don);
	printer(3, coder->id, coder->shared);
	pthread_cond_signal(&coder->r_don->cond);
	return (0);
}

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

int	take_dong(t_dongle *x_don, t_coder *x_coder)
{
	pthread_mutex_lock(&x_don->mutex);
	if (x_don->available == 0)
	{
		x_don->available = 1;
		x_coder->ndong += 1;
	}
	else
	
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

void is_available(t_dongle *x_dong)
{
	pthread_mutex_lock(&x_dong->mutex);
	x_dong->available = !(x_dong->available);
	pthread_mutex_unlock(&x_dong->mutex);
}

int	is_sim(t_shared *g_info)
{
	int	i;

	pthread_mutex_lock(&g_info->end_mutex);
	i = g_info->simulation_end;
	pthread_mutex_unlock(&g_info->end_mutex);
	return (i);
}

void the_dongles(t_coder *coder)
{
	//si coder-> i -1 es negativo -> coger derecha y luego izquierda. Else izquierda primero.+
	//take dongle del que sea. y luego intentar coger el otro. Tener un avisador de cuantos dongles tiene cogisdos. Si tiene 1 dongle liberar y el otro esta cogido, primero soltar, cambiar el avisador y 
	// ponerme en espera.
	if (coder->id - 1 > 0)
	{
		if (take_dong(coder->l_don, coder) == 1)
			return (1);
		printer(2, coder->id, coder->shared);
		if (take_dong(coder->r_don, coder) == 1)
		{
			pthread_cond_signal(&coder->r_don->cond);
			is_available(coder->l_don);
			return(1);
		}
		if (coder->ndong != 2)
			i
	}
	else 
}

