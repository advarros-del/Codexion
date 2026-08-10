/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrvarga <adrvarga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 12:29:36 by adrvarga          #+#    #+#             */
/*   Updated: 2026/08/10 20:00:21 by adrvarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

int	fifo_thing(t_dongle *x_dong, int coders)
{
	int				i;
	long long int	aux;
	int				id_aux;

	i = 0;
	aux = 2147483647;
	while (i < coders)
	{
		if (x_dong->requests[i].time_stamp > 0 && x_dong->requests[i].time_stamp < aux)
		{
			aux = x_dong->requests[i].time_stamp;
			id_aux = x_dong->requests[i].id_code;
		}
		i++;
	}
	x_dong->requests[id_aux].time_stamp = 0;
	return (id_aux);
}

void	requesting(t_dongle *x_don, t_coder *x_coder)
{
	if (x_coder->shared->data->scheduler == 1)
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