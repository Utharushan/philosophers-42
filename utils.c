/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:57:11 by tuthayak          #+#    #+#             */
/*   Updated: 2025/03/19 11:57:11 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(int time)
{
	long long	start_time;

	start_time = get_time_in_ms();
	while (get_time_in_ms() - start_time < time)
		usleep(100);
}

void	print_action(t_philo *philo, char *msg)
{
	long long	timestamp;

	timestamp = get_time_in_ms() - philo->data->start_time;
	pthread_mutex_lock(&philo->data->print_lock);
	printf("%lld %d %s\n", timestamp, philo->id, msg);
	pthread_mutex_unlock(&philo->data->print_lock);
}
