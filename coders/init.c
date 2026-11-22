/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 21:02:52 by adrian            #+#    #+#             */
/*   Updated: 2026/11/22 21:39:35 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int init_donglers(t_dongle *donglers, int total)
{
    int i;

    i = 0;
    while (i < total)
    {
        donglers[i].requests =
            malloc(sizeof(t_heap_call) * total);
        if (!donglers[i].requests)
            break ;
        if (pthread_mutex_init(&donglers[i].mutex, NULL) != 0)
        {
            free(donglers[i].requests);
            break ;
        }
        if (pthread_cond_init(&donglers[i].cond, NULL) != 0)
        {
            pthread_mutex_destroy(&donglers[i].mutex);
            free(donglers[i].requests);
            break ;
        }
        i++;
    }
    if (i != total)
        clean_donglers(donglers, i);
    return (i);
}

int  init_coder(t_shared *g_info)
{
    int i;

    i = 0;
    while (i < g_info->data->coders)
    {
        if (pthread_mutex_init(&g_info->coders[i].mutex, NULL) != 0)
        {
            clean_coders(g_info->coders, i);
            return (-1);
	    }
        i++;
    }
    return (i);
}