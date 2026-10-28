/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 12:29:36 by adrvarga          #+#    #+#             */
/*   Updated: 2026/10/28 22:28:06 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

int	fifo_thing(t_dongle *x_dong, int coders, int id_coder)
{
	int				i;
	long long int	aux;
	int				id_aux;

	i = 0;
	aux = 9223372036854775807;
	while (i < coders)
	{
		if (x_dong->requests[i].time_stamp > 0 && x_dong->requests[i].time_stamp < aux)
		{
			aux = x_dong->requests[i].time_stamp;
			id_aux = x_dong->requests[i].id_code;
		}
		i++;
	}
	if (id_coder == id_aux)
		x_dong->requests[id_aux].time_stamp = 0;
	return (id_aux);
}

void	requesting(t_dongle *x_don, t_coder *x_coder)
{
	if (x_coder->shared->data->scheduler == 0)
	{
		x_don->requests[x_coder->id].id_code = x_coder->id;
		if (x_don->requests[x_coder->id].time_stamp <= 0)
			x_don->requests[x_coder->id].time_stamp = gettime();
	}
	else
	{
		x_don->requests[x_coder->id].id_code = x_coder->id;
		if (x_don->requests[x_coder->id].deadline <= 0)
		x_don->requests[x_coder->id].deadline = x_coder->last_compile + x_coder->shared->data->burntime;
	}
}

int edf_thing(t_dongle *x_dong, int coders, int id_coders)
{
	int				i;
	long long int	aux;
	int				id_aux;

	i = 0;
	aux = 9223372036854775807;
	while (i < coders)
	{
		if (x_dong->requests[i].deadline > 0 && x_dong->requests[i].deadline < aux)
		{
			aux = x_dong->requests[i].deadline;
			id_aux = x_dong->requests[i].id_code;
		}
		i++;
	}
	if (id_coders == id_aux)
		x_dong->requests[id_aux].deadline = 0;
	return (id_aux);
}