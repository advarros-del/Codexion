/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrvarga <adrvarga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 14:57:59 by adrvarga          #+#    #+#             */
/*   Updated: 2026/07/13 19:58:57 by adrvarga         ###   ########.fr       */
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
	long long	compile_time;
	long long	debug_time;
	long long	refac_time;
	int	n_compile;
	int	dongle_cd;
	int	scheduler;
}	t_data;

typedef struct dongle
{
	int				used;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	long			last_release;
	int				available;

}	t_dongle;

typedef struct coder
{
	int				id;
	int				compile_counter;
	int				last_compile;
	int				status;
	pthread_t		thread;
	t_dongle		*l_don;
	t_dongle		*r_don;
	struct share	*shared;
}	t_coder;

typedef struct share
{
	t_data			*data;
	long			start_ime;
	int				simulation_end;
	pthread_mutex_t	end_mutex;
	pthread_cond_t	end_cond;
	pthread_mutex_t	print_mutex;
	t_coder			*coders;
	t_dongle		*donglers;

}	t_shared;

typedef struct heap_call
{
	int	id_code;
	int	time_stamp;
	int	deadline;
}	t_heap_call;

typedef struct don_container
{
	void	*array;
	int		counter;
}	t_dcontainer;



int			fill_things(t_shared *g_info, t_coder *the_coders, t_dongle *donglers);
long int	ft_atol(const char *nptr);
long long	gettime(void);
void	sleepnow(long long time);
void		*coder_routine(void *arg);
int			codexion(char **argv);
#endif
