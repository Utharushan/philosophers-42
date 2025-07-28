/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 07:57:10 by tuthayak          #+#    #+#             */
/*   Updated: 2025/06/06 07:57:10 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philos(t_data *data, t_philo *philos)
{
	int		i;

	i = 0;
	while (i < data->num_philos)
	{
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = data->start_time;
		philos[i].data = data;
		philos[i].left_fork = &data->forks[i];
		philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		philos[i].done_eating = 0;
		i++;
	}
	return (0);
}

int	create_threads(t_data *data, t_philo *philos, pthread_t *threads)
{
	int		i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&threads[i], NULL, philo_routine, &philos[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

void	print_death(t_data *data, t_philo *philo)
{
	char		buf[64];
	int			len;
	long long	timestamp;
	char		*msg;
	int			j;

	len = 0;
	timestamp = get_time() - data->start_time;
	timestamp_to_str(timestamp, buf, &len);
	buf[len++] = ' ';
	id_to_str(philo->id, buf, &len);
	buf[len++] = ' ';
	msg = "died";
	j = 0;
	while (msg[j])
		buf[len++] = msg[j++];
	buf[len++] = '\n';
	write(1, buf, len);
}

int	check_philo_death(t_data *data, t_philo *philo)
{
	long long	current_time;
	long long	last_meal_time;
	int			philo_died;

	current_time = get_time();
	pthread_mutex_lock(&data->state_mutex);
	if (data->died)
	{
		pthread_mutex_unlock(&data->state_mutex);
		return (0);
	}
	last_meal_time = philo->last_meal;
	philo_died = (current_time - last_meal_time > data->time_to_die);
	if (philo_died)
		data->died = 1;
	pthread_mutex_unlock(&data->state_mutex);
	if (philo_died)
	{
		pthread_mutex_lock(&data->print_mutex);
		print_death(data, philo);
		pthread_mutex_unlock(&data->print_mutex);
		return (1);
	}
	return (0);
}

int	check_all_philos_death(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (check_philo_death(data, &philos[i]))
			return (1);
		i++;
	}
	return (0);
}
