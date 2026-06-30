/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrvarga <adrvarga@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:52:55 by adrvarga          #+#    #+#             */
/*   Updated: 2026/03/04 15:31:04 by adrvarga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
/************************************************************************
 * atol transform an ascii number into a regular number in the long int *
 ***********************************************************************/

long int	ft_atol(const char *nptr)
{
	int			i;
	long int	dest;
	long int	n;

	dest = 0;
	n = 1;
	i = 0;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if ((nptr[i] == '+' && nptr[i + 1] == '-')
		|| (nptr[i] == '-' && nptr[i + 1] == '+'))
		return (0);
	if (nptr[i] == '+')
		i++;
	if (nptr[i] == '-')
	{
		n = -n;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		dest = (dest * 10) + (nptr[i] - '0');
		i++;
	}
	return (dest * n);
}

