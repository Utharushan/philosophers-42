/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:30:00 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/28 17:30:00 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_action(t_data *data, int id, char *message)
{
	long long	timestamp;
	char		buf[128];
	int			len;
	int			i;

	pthread_mutex_lock(&data->print_mutex);
	timestamp = get_time() - data->start_time;
	if (!data->died)
	{
		len = 0;
		timestamp_to_str(timestamp, buf, &len);
		buf[len++] = ' ';
		id_to_str(id, buf, &len);
		buf[len++] = ' ';
		i = 0;
		while (message[i])
			buf[len++] = message[i++];
		buf[len++] = '\n';
		write(1, buf, len);
	}
	pthread_mutex_unlock(&data->print_mutex);
}
