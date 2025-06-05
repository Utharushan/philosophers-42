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

void	ft_usleep(int time_ms)
{
	long long	start;

	start = get_time_in_ms();
	while ((get_time_in_ms() - start) < time_ms)
		usleep(100);
}

void	print_action(t_philo *philo, const char *msg)
{
	long long	ts;

	if (has_simulation_stopped(philo->data))
		return ;
	pthread_mutex_lock(&philo->data->print_lock);
	ts = get_time_in_ms() - philo->data->start_time;
	if (!philo->data->stop_simulation)
		printf("%lld %d %s\n", ts, philo->id, msg);
	pthread_mutex_unlock(&philo->data->print_lock);
}

void start_simulation(t_data *data)
{
    int i;
    pthread_t monitor;

    data->start_time = get_time_in_ms();
    for (i = 0; i < data->num_philos; i++)
    {
        data->philos[i].last_meal_time = data->start_time;
        pthread_create(&data->philos[i].thread, NULL, philosopher_routine, &data->philos[i]);
    }
    pthread_create(&monitor, NULL, grim_reaper, data);
    pthread_join(monitor, NULL);
    for (i = 0; i < data->num_philos; i++)
        pthread_join(data->philos[i].thread, NULL);
}