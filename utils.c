/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 22:49:43 by tuthayak          #+#    #+#             */
/*   Updated: 2025/03/20 22:49:43 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	has_simulation_stopped(t_data *data)
{
	bool	result;

	result = false;
	pthread_mutex_lock(&data->death_lock);
	if (data->someone_died
		|| (data->max_meals > 0 && data->full_count >= data->num_philos))
		result = true;
	pthread_mutex_unlock(&data->death_lock);
	return (result);
}

bool	end_condition_reached(t_data *data)
{
	int			i;
	long long	time;

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
			set_sim_stop_flag(data, true);
			return (true);
		}
		if (data->max_meals > 0 && data->full_count >= data->num_philos)
		{
			pthread_mutex_unlock(&data->death_lock);
			set_sim_stop_flag(data, true);
			return (true);
		}
		pthread_mutex_unlock(&data->death_lock);
	}
	return (false);
}

void	*lone_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_action(philo, "has taken a fork");
	ft_usleep(philo->data->time_to_die);
	pthread_mutex_lock(&philo->data->death_lock);
	if (!philo->data->someone_died)
	{
		print_action(philo, "died");
		philo->data->someone_died = 1;
	}
	pthread_mutex_unlock(&philo->data->death_lock);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

void	set_sim_stop_flag(t_data *data, bool state)
{
	pthread_mutex_lock(&data->death_lock);
	data->someone_died = state;
	pthread_mutex_unlock(&data->death_lock);
}

void	think_routine(t_philo *philo, bool silent)
{
	time_t	time_to_think;

	pthread_mutex_lock(&philo->data->death_lock);
	time_to_think = (philo->data->time_to_die
			- (get_time_in_ms() - philo->last_meal_time)
			- philo->data->time_to_eat) / 2;
	pthread_mutex_unlock(&philo->data->death_lock);
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && silent == true)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (!silent)
		print_action(philo, "is thinking");
	ft_usleep(time_to_think);
}
