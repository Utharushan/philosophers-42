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

static void philosopher_eat(t_philo *philo)
{
	t_data *data = philo->data;

	if (has_simulation_stopped(data))
		return;

	pthread_mutex_lock(philo->left_fork);
	print_action(philo, "has taken a fork");

	pthread_mutex_lock(philo->right_fork);
	print_action(philo, "has taken a fork");

	pthread_mutex_lock(&data->death_lock);
	philo->last_meal_time = get_time_in_ms();
	philo->eat_count++;
	if (data->max_meals > 0 && philo->eat_count == data->max_meals)
		data->full_count++;
	pthread_mutex_unlock(&data->death_lock);

	print_action(philo, "is eating");
	ft_usleep(data->time_to_eat);

	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static void philosopher_sleep_and_think(t_philo *philo)
{
	t_data *data = philo->data;

	if (has_simulation_stopped(data))
		return;

	print_action(philo, "is sleeping");
	ft_usleep(data->time_to_sleep);

	if (has_simulation_stopped(data))
		return;

	print_action(philo, "is thinking");
}

void	*philosopher_routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	t_data  *data  = philo->data;

	if (data->num_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork");
		ft_usleep(data->time_to_die);
		print_action(philo, "died");
		set_simulation_stopped(data);
		pthread_mutex_unlock(philo->left_fork);
		return NULL;
	}

	if (philo->id % 2 == 1)
		usleep(100);

	while (!has_simulation_stopped(data))
	{
		philosopher_eat(philo);
		if (has_simulation_stopped(data))
			break;
		philosopher_sleep_and_think(philo);
	}
	return NULL;
}

void *grim_reaper(void *arg)
{
	t_data *data = (t_data *)arg;

	set_simulation_stopped(data); // ensure stop_simulation is 0 at start
	pthread_mutex_lock(&data->death_lock);
	data->stop_simulation = 0;
	pthread_mutex_unlock(&data->death_lock);

	while (!has_simulation_stopped(data))
	{
		if (end_condition_reached(data))
			break;
		usleep(500);
	}
	return NULL;
}

