/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compilation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 12:29:36 by adrvarga          #+#    #+#             */
/*   Updated: 2026/10/26 21:29:29 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/***************************************************
* -Check both dongles are free.                    *
* -If not, return error, if yes, take both dongles *
************************************************* */

int	take_dongles(t_coder *coder)
{
	if (coder->id == 0)
	{
		pthread_mutex_lock(&coder->r_don->mutex);
		pthread_mutex_lock(&coder->l_don->mutex);
	}
	else
	{
		pthread_mutex_lock(&coder->l_don->mutex);
		pthread_mutex_lock(&coder->r_don->mutex);
	}
	if (coder->r_don->available != 0)
	{
		pthread_mutex_unlock(&coder->r_don->mutex);
		pthread_mutex_unlock(&coder->l_don->mutex);
		return (1);
	}
	else if (coder->l_don->available != 0)
	{
		pthread_mutex_unlock(&coder->r_don->mutex);
		pthread_mutex_unlock(&coder->l_don->mutex);
		return (2);
	}
	coder->r_don->available = 1;
	pthread_mutex_unlock(&coder->r_don->mutex);
	printer(2, coder->id, coder->shared);
	coder->l_don->available = 1;
	pthread_mutex_unlock(&coder->l_don->mutex);
	printer(2, coder->id, coder->shared);
	return (0);
}

/***************************************************
* -Change the available value                      *
************************************************* */
void is_available(t_dongle *x_dong)
{
	pthread_mutex_lock(&x_dong->mutex);
	x_dong->available = !(x_dong->available);
	pthread_mutex_unlock(&x_dong->mutex);
}

/***************************************************
* -Send to sleep the coders until signal call them.*
* If they are not selected, and sim is not end,    *
*  sleep again.                                    *
************************************************* */

int go_to_sleep(t_coder *coder, t_dongle *x_dongle)
{
	pthread_mutex_lock(&x_dongle->mutex);
    while(1)
    {
		requesting(x_dongle, coder);
        pthread_cond_wait(&x_dongle->cond, &x_dongle->mutex);
		if (is_sim(coder->shared) == 1)
		{
			pthread_mutex_unlock(&x_dongle->mutex);
			return (1);
		}
        if (coder->shared->data->scheduler == 0)
        {
            if (fifo_thing(x_dongle, coder->shared->data->coders, coder->id) == coder->id)
                break;
        }
        else
        {
            if (edf_thing(x_dongle, coder->shared->data->coders, coder->id) == coder->id)
                break;
        }
	}
	pthread_mutex_unlock(&x_dongle->mutex);
	return (0);
}

/***************************************************
* -Compiling rutine, take dongles, send print,     *
*  leave, dongle, send signal                      *
************************************************* */

int   compiling(t_coder *coder)
{
	int aux;
	
	aux = take_dongles(coder);
	while (aux != 0)
	{
		if (aux == 1)
		{
			if (go_to_sleep(coder, coder->r_don) == 1)
				return (1);
		}
		else if (aux == 2)
		{
			if (go_to_sleep(coder, coder->l_don) == 1)
				return (1);
		}
		aux = take_dongles(coder);
	}
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