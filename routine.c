/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 07:56:54 by tuthayak          #+#    #+#             */
/*   Updated: 2025/06/06 07:56:54 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_action(philo->data, philo->id, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_action(philo->data, philo->id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_action(philo->data, philo->id, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_action(philo->data, philo->id, "has taken a fork");
	}
}

void	eat_and_update(t_philo *philo)
{
	print_action(philo->data, philo->id, "is eating");
	pthread_mutex_lock(&philo->data->state_mutex);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	if (philo->data->must_eat > 0 && philo->meals_eaten == philo->data->must_eat
		&& !philo->done_eating)
	{
		philo->data->all_ate_count++;
		philo->done_eating = 1;
	}
	if (philo->data->all_ate_count == philo->data->num_philos)
		philo->data->all_ate = 1;
	pthread_mutex_unlock(&philo->data->state_mutex);
	ft_usleep(philo->data->time_to_eat, philo->data);
}

void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	sleep_and_think(t_philo *philo)
{
	print_action(philo->data, philo->id, "is sleeping");
	ft_usleep(philo->data->time_to_sleep, philo->data);
	print_action(philo->data, philo->id, "is thinking");
}

int	should_stop(t_philo *philo)
{
	int	stop;

	pthread_mutex_lock(&philo->data->state_mutex);
	stop = (philo->data->died
			|| (philo->data->must_eat > 0 && philo->data->all_ate));
	pthread_mutex_unlock(&philo->data->state_mutex);
	return (stop);
}
