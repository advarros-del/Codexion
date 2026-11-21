/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compilation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 12:29:36 by adrvarga          #+#    #+#             */
/*   Updated: 2026/11/21 21:22:33 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
/***************************************************
* -Mute on the correct order                       *
***************************************************/

static void	mute_this(t_coder *coder, int r)
{
	if (r == 1)
	{
		pthread_mutex_lock(&coder->r_don->mutex);
		pthread_mutex_lock(&coder->l_don->mutex);
	}
	else
	{
		pthread_mutex_lock(&coder->l_don->mutex);
		pthread_mutex_lock(&coder->r_don->mutex);
	}
}

/***************************************************
* -Check both dongles are free.                    *
* -If not, return error, if yes, take both dongles *
***************************************************/

int	take_dongles(t_coder *coder)
{
	if (coder->id == 0)
		mute_this(coder, 1);
	else
		mute_this(coder, 0);
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
	printer(1, coder->id, coder->shared);
	coder->l_don->available = 1;
	pthread_mutex_unlock(&coder->l_don->mutex);
	printer(1, coder->id, coder->shared);
	return (0);
}

/***************************************************
* -Change the available value                      *
***************************************************/


/***************************************************
* -Send to sleep the coders until signal call them.*
* If they are not selected, and sim is not end,    *
*  sleep again.                                    *
***************************************************/

int	go_to_sleep(t_coder *coder, t_dongle *x_dongle)
{
	pthread_mutex_lock(&x_dongle->mutex);
	while (1)
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
			if (fifo_thing(x_dongle, coder->shared->data->coders,
					coder->id) == coder->id)
				break ;
		}
		else
		{
			if (edf_thing(x_dongle, coder->shared->data->coders,
					coder->id) == coder->id)
				break ;
		}
	}
	pthread_mutex_unlock(&x_dongle->mutex);
	return (0);
}

/***************************************************
* -Compiling rutine, take dongles, send print,     *
*  leave, dongle, send signal                      *
***************************************************/

int	compiling(t_coder *coder)
{
	int	aux;

	aux = take_dongles(coder);
	while (aux != 0)
	{
		if (aux == 1 && go_to_sleep(coder, coder->r_don) == 1)
			return (1);
		else if (aux == 2 && go_to_sleep(coder, coder->l_don) == 1)
			return (1);
		aux = take_dongles(coder);
	}
	if (still_alive(coder))
		return (1);
	pthread_mutex_lock(&coder->mutex);
	coder->status = 2;
	coder->compile_counter += 1;
	coder->last_compile = gettime();
	pthread_mutex_unlock(&coder->mutex);
	printer(3, coder->id, coder->shared);
	usleep(coder->shared->data->compile_time * 1000);
	free_dongle(coder);
	pthread_mutex_lock(&coder->mutex);
	coder->status = 0;
	pthread_mutex_unlock(&coder->mutex);
	return (0);
}

