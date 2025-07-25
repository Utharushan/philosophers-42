/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 20:56:06 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/25 20:56:06 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_philo_death(t_philo *philo)
{
	char		buf[64];
	int			len;
	long long	timestamp;
	char		*msg;
	int			i;

	len = 0;
	timestamp = get_time() - philo->data->start_time;
	timestamp_to_str(timestamp, buf, &len);
	buf[len++] = ' ';
	id_to_str(philo->id, buf, &len);
	buf[len++] = ' ';
	msg = "died";
	i = 0;
	while (msg[i])
		buf[len++] = msg[i++];
	buf[len++] = '\n';
	write(1, buf, len);
}

void	handle_one_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_action(philo->data, philo->id, "has taken a fork");
	ft_usleep(philo->data->time_to_die, philo->data);
	pthread_mutex_lock(&philo->data->state_mutex);
	philo->data->died = 1;
	pthread_mutex_unlock(&philo->data->state_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	print_philo_death(philo);
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_unlock(philo->left_fork);
}
