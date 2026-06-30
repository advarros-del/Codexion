/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrvarga <adrvarga@student.42malaga.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 14:57:59 by adrvarga          #+#    #+#             */
/*   Updated: 2026/06/23 14:57:59 by adrvarga         ###   ########.fr       */
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
}	t_data;


long int	ft_atol(const char *nptr);
int			codexion(char **argv);
#endif
