/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:52:55 by adrvarga          #+#    #+#             */
/*   Updated: 2026/11/20 15:07:15 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
/************************************************************************
 * atol transform an ascii number into a regular number in the long int *
 ***********************************************************************/

long int	ft_atol(const char *nptr)
{
	int			i;
	long int	dest;
	long int	n;

	dest = 0;
	n = 1;
	i = 0;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if ((nptr[i] == '+' && nptr[i + 1] == '-')
		|| (nptr[i] == '-' && nptr[i + 1] == '+'))
		return (0);
	if (nptr[i] == '+')
		i++;
	if (nptr[i] == '-')
	{
		n = -n;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		dest = (dest * 10) + (nptr[i] - '0');
		i++;
	}
	return (dest * n);
}

int	is_debugging(t_coder *coder)//comprobar que sigue vivo
{
	long long int	now;

	now = gettime();
	pthread_mutex_lock(&coder->mutex);
	if (coder->status == 1 || is_sim(coder->shared) == 1
		|| now - coder->last_compile >= coder->shared->data->burntime)
	{
		pthread_mutex_unlock(&coder->mutex);
		return (1);
	}
	pthread_mutex_unlock(&coder->mutex);
	printer(4, coder->id, coder->shared);
	usleep(coder->shared->data->debug_time * 1000);
	return (0);
}

int	is_refactoring(t_coder *coder)
{
	long long int	now;

	now = gettime();
	pthread_mutex_lock(&coder->mutex);
	if (coder->status == 1 || is_sim(coder->shared) == 1
		|| now - coder->last_compile >= coder->shared->data->burntime)
	{
		pthread_mutex_unlock(&coder->mutex);
		return (1);
	}
	pthread_mutex_unlock(&coder->mutex);
	printer(5, coder->id, coder->shared);
	usleep(coder->shared->data->refac_time * 1000);
	return (0);
}
