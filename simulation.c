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
	if (has_simulation_stopped(philo->data))
		return ;
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
	if (!has_simulation_stopped(philo->data))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}

void	philo_sleep_think(t_philo *philo)
{
	if (has_simulation_stopped(philo->data))
		return ;
	print_action(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
	if (has_simulation_stopped(philo->data))
		return ;
	print_action(philo, "is thinking");
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->num_philos == 1)
		return (lone_philo_routine(philo));
	if (philo->id % 2)
		think_routine(philo, true);
	while (!has_simulation_stopped(philo->data))
	{
		philo_eat(philo);
		if (has_simulation_stopped(philo->data))
			break;
		philo_sleep_think(philo);
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
	pthread_create(&monitor, NULL, grim_reaper, (void *)data);
	pthread_join(monitor, NULL);
	i = -1;
	while (++i < data->num_philos)
		pthread_join(data->philos[i].thread, NULL);
}

void	*grim_reaper(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	if (data->max_meals == 0)
		return (NULL);
	set_sim_stop_flag(data, false);
	while (!has_simulation_stopped(data))
	{
		if (end_condition_reached(data))
		{
			set_sim_stop_flag(data, true);
			break ;
		}
		usleep(500);
	}
	return (NULL);
}
