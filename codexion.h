/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrvarga <adrvarga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 14:57:59 by adrvarga          #+#    #+#             */
/*   Updated: 2026/06/25 14:27:49 by adrvarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>


typedef struct data
{
	int	coders;
	int	burntime;
	int	compile_time;
	int	debug_time;
	int	refac_time;
	int	n_compile;
	int	dongle_cd;
	int	scheduler;
}

	t_data;

typedef struct coder
{
	int id;
	int compiler_done;
	int last_compile;
	t_dongle *l_dong;
	t_dongle *r_dong;
	t_shared *shared;
	
}
	t_coder;

typedef struct share
{
	int	start_time;
	t_data *data;
	long start_ime;
	int simulation_end;
	pthread_mutex_t print_mutex;
	t_coder *coders;
	t_dongle *donglers;
	
}
	t_shared;

typedef struct dongle
{
	int used;
	pthread_mutex_t mutex;
	long last_release;
	int available;
	
}
	t_dongle;


long int	ft_atol(const char *nptr);
#endif
