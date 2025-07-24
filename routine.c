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

static void	handle_one_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_action(philo->data, philo->id, "has taken a fork");
	ft_usleep(philo->data->time_to_die, philo->data);
	pthread_mutex_lock(&philo->data->state_mutex);
	philo->data->died = 1;
	pthread_mutex_unlock(&philo->data->state_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	{
		char buf[64];
		int len = 0;
		long long timestamp = get_time() - philo->data->start_time;
		timestamp_to_str(timestamp, buf, &len);
		buf[len++] = ' ';
		id_to_str(philo->id, buf, &len);
		buf[len++] = ' ';
		char *msg = "died";
		int i = 0;
		while (msg[i])
			buf[len++] = msg[i++];
		buf[len++] = '\n';
		write(1, buf, len);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_unlock(philo->left_fork);
}

static void	take_forks(t_philo *philo)
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

static void	eat_and_update(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->state_mutex);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	if (philo->data->must_eat > 0 && philo->meals_eaten == philo->data->must_eat && !philo->done_eating)
	{
		philo->data->all_ate_count++;
		philo->done_eating = 1;
	}
	if (philo->data->all_ate_count == philo->data->num_philos)
		philo->data->all_ate = 1;
	pthread_mutex_unlock(&philo->data->state_mutex);
	print_action(philo->data, philo->id, "is eating");
	ft_usleep(philo->data->time_to_eat, philo->data);
}

static void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static void	sleep_and_think(t_philo *philo)
{
	print_action(philo->data, philo->id, "is sleeping");
	ft_usleep(philo->data->time_to_sleep, philo->data);
	print_action(philo->data, philo->id, "is thinking");
}

static int	should_stop(t_philo *philo)
{
	int stop;

	pthread_mutex_lock(&philo->data->state_mutex);
	stop = (philo->data->died || (philo->data->must_eat && philo->data->all_ate));
	pthread_mutex_unlock(&philo->data->state_mutex);
	return (stop);
}

void	*philo_routine(void *arg)
{
	t_philo *philo;
	t_data *data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (data->num_philos == 1)
	{
		handle_one_philo(philo);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(100);
	while (1)
	{
		if (should_stop(philo))
			break;
		take_forks(philo);
		eat_and_update(philo);
		release_forks(philo);
		sleep_and_think(philo);
		if (should_stop(philo))
			break;
	}
	return (NULL);
}
