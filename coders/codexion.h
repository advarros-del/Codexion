/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 14:57:59 by adrvarga          #+#    #+#             */
/*   Updated: 2026/10/26 20:01:15 by adrian           ###   ########.fr       */
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

typedef struct heap_call
{
	int	id_code;
	long long	time_stamp;
	long long	deadline;
}	t_heap_call;

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
	t_heap_call		*requests;

}	t_dongle;

typedef struct coder
{
	int				id;
	int				compile_counter;
	long long		last_compile;
	int				status;
	int				ndong;
	pthread_t		thread;
	t_dongle		*l_don;
	t_dongle		*r_don;
	struct share	*shared;
	pthread_mutex_t	mutex;
}	t_coder;

typedef struct share
{
	t_data			*data;
	long long		start_ime;
	int				simulation_end;
	pthread_mutex_t	end_mutex;
	pthread_cond_t	end_cond;
	pthread_mutex_t	print_mutex;
	t_coder			*coders;
	t_dongle		*donglers;

}	t_shared;


typedef struct don_container
{
	void	*array;
	int		counter;
}	t_dcontainer;



int			fill_things(t_shared *g_info, t_coder *the_coders, t_dongle *donglers);
void		printer(int phrase, int coder, t_shared *g_info);
long int	ft_atol(const char *nptr);
long long	gettime(void);
void		sleepnow(long long time);
void		*coder_routine(void *arg);
void		*monitor_rutine(void *arg);
int			codexion(char **argv);
int			compiling(t_coder *coder);
int			take_dong(t_dongle *x_don,  t_coder *x_coder);
void		is_available(t_dongle *x_dong);
int			is_sim(t_shared *g_info);
int			is_sim_ended(t_shared* g_info);
int			fifo_thing(t_dongle *x_dong, int coders, int id_coder);
int 		edf_thing(t_dongle *x_dong, int coders, int id_coders);
int			go_to_sleep(t_coder *coder, t_dongle *x_dong);
void		requesting(t_dongle *x_don, t_coder *x_coder);
int			on_fire(t_coder *coders);
#endif
