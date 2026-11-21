/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compi_aux.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/21 18:02:08 by adrian            #+#    #+#             */
/*   Updated: 2026/11/21 20:23:15 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	is_available(t_dongle *x_dong, long int dongle_cd)
{
	pthread_mutex_lock(&x_dong->mutex);
	if (x_dong->available == 0)
	{
		x_dong->available = !(x_dong->available);
		pthread_mutex_unlock(&x_dong->mutex);
	}
	else
	{
		x_dong->available = !(x_dong->available);
		pthread_mutex_unlock(&x_dong->mutex);
		usleep(dongle_cd * 1000);
	}
}

void	free_dongle(t_coder *coder)
{
	is_available(coder->l_don, coder->shared->data->dongle_cd);
	printer(2, coder->id, coder->shared);
	pthread_cond_signal(&coder->l_don->cond);
	is_available(coder->r_don, coder->shared->data->dongle_cd);
	printer(2, coder->id, coder->shared);
	pthread_cond_signal(&coder->r_don->cond);
}

int	still_alive(t_coder *coder)
{
	long long int	now;

	now = gettime();
	pthread_mutex_lock(&coder->mutex);
	if (coder->status == 1
		|| now - coder->last_compile >= coder->shared->data->burntime)
	{
		pthread_mutex_unlock(&coder->mutex);
		free_dongle(coder);
		return (1);
	}
	pthread_mutex_unlock(&coder->mutex);
	return (0);
}