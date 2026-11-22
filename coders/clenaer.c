/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clenaer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 21:05:40 by adrian            #+#    #+#             */
/*   Updated: 2026/11/22 21:29:08 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void in_the_end(t_shared *g_info)
{
	int	i;

	i = 0;
	while (i < g_info->data->coders)
	{
		if (g_info->donglers[i].requests != NULL)
			free(g_info->donglers[i].requests);
		pthread_mutex_destroy(&g_info->coders[i].mutex);
		pthread_mutex_destroy(&g_info->donglers[i].mutex);
		pthread_cond_destroy(&g_info->donglers[i].cond);
		i++;
	}
	pthread_mutex_destroy(&g_info->end_mutex);
	pthread_mutex_destroy(&g_info->print_mutex);
	if (g_info->coders != NULL)
		free(g_info->coders);
	if (g_info->donglers != NULL)
		free(g_info->donglers);
	if (g_info->data != NULL)
		free(g_info->data);
}

void    clean_coders(t_coder *coders, int i)
{
    while (i > 0)
    {
        i--;
        pthread_mutex_destroy(&coders[i].mutex);
    }
    free(coders);
}

void    clean_donglers(t_dongle *donglers, int i)
{
    while (i > 0)
    {
        i--;
        pthread_cond_destroy(&donglers[i].cond);
        pthread_mutex_destroy(&donglers[i].mutex);
        free(donglers[i].requests);
        donglers[i].requests = NULL;
    }
    free(donglers);
}

void clean_shared(t_shared *g_info)
{
    pthread_mutex_destroy(&g_info->print_mutex);
    pthread_mutex_destroy(&g_info->end_mutex);
}