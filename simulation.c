/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:56:49 by tuthayak          #+#    #+#             */
/*   Updated: 2025/03/19 11:56:49 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_action(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_fork);
	print_action(philo, "has taken a fork");
	print_action(philo, "is eating");
	pthread_mutex_lock(&philo->data->death_lock);
	philo->last_meal_time = get_time_in_ms();
	philo->eat_count++;
	if (philo->eat_count == philo->data->max_meals)
		philo->data->full_count++;
	pthread_mutex_unlock(&philo->data->death_lock);
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	philo_sleep_think(t_philo *philo)
{
	print_action(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
	print_action(philo, "is thinking");
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		philo_eat(philo);
		philo_sleep_think(philo);
		if (philo->data->full_count == philo->data->num_philos)
			break ;
	}
	return (NULL);
}

void	start_simulation(t_data *data)
{
	int			i;
	pthread_t	monitor;

	data->start_time = get_time_in_ms();
	i = -1;
	while (++i < data->num_philos)
		pthread_create(&data->philos[i].thread, NULL,
			philosopher_routine, &data->philos[i]);
	pthread_create(&monitor, NULL, check_death, (void *)data);
	pthread_join(monitor, NULL);
	i = -1;
	while (++i < data->num_philos)
		pthread_join(data->philos[i].thread, NULL);
}

void	*check_death(void *arg) // Corrected return type
{
	t_data		*data;
	int			i;
	long long	time;

	data = (t_data *)arg;
	while (1)
	{
		i = -1;
		while (++i < data->num_philos)
		{
			pthread_mutex_lock(&data->death_lock);
			time = get_time_in_ms() - data->philos[i].last_meal_time;
			if (time >= data->time_to_die)
			{
				print_action(&data->philos[i], "died");
				data->someone_died = 1;
				pthread_mutex_unlock(&data->death_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&data->death_lock);
		}
		usleep(1000);
	}
	return (NULL);
}
