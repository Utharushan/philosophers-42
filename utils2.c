/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 22:14:25 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/24 22:22:03 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	timestamp_to_str(long long timestamp, char *buf, int *len)
{
	char	rev[32];
	int		idx;

	idx = 0;
	if (timestamp == 0)
		buf[(*len)++] = '0';
	else
	{
		while (timestamp > 0)
		{
			rev[idx++] = (timestamp % 10) + '0';
			timestamp /= 10;
		}
		while (idx > 0)
			buf[(*len)++] = rev[--idx];
	}
}

void	id_to_str(int id, char *buf, int *len)
{
	char	rev2[16];
	int		idx2;

	idx2 = 0;
	if (id == 0)
		buf[(*len)++] = '0';
	else
	{
		while (id > 0)
		{
			rev2[idx2++] = (id % 10) + '0';
			id /= 10;
		}
		while (idx2 > 0)
			buf[(*len)++] = rev2[--idx2];
	}
}
