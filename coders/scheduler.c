/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrvarga <adrvarga@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 12:29:36 by adrvarga          #+#    #+#             */
/*   Updated: 2026/08/10 12:29:36 by adrvarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

//void fifo_thing(t_shared *g_info)
//{
//	g_info->scheduler
//}

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