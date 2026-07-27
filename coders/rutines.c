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
	if (coder->l_don->available == 0)
		{
			coder->l_don->available = 1;
			printf("the coder %d take a dongler", coder->id);
		}
	if (coder->r_don->available == 0)
		{
			coder->r_don->available = 1;
			printf("the coder %d take a dongler", coder->id);
		}
	printf("coder %d compiling", coder->id);
	coder->compile_counter += 1;
	coder->last_compile = gettime();
	sleepnow(coder->shared->data->compile_time);
	coder->l_don->available = 0;
	coder->r_don->available = 0;
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
		//if (is_sim_ended(coder) == 1)
		//	break;
		compiling(coder);
		//if (is_sim_ended() == 1)
		//	break;
		is_debugging(coder);
		is_refactoring(coder);
	}
	return (NULL);
}