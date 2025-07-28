/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:30:00 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/28 17:30:00 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	run_philo_loop(t_philo *philo)
{
	while (1)
	{
		if (should_stop(philo))
			break ;
		take_forks(philo);
		eat_and_update(philo);
		release_forks(philo);
		sleep_and_think(philo);
		if (should_stop(philo))
			break ;
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (data->num_philos == 1)
	{
		handle_one_philo(philo);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(1000);
	run_philo_loop(philo);
	return (NULL);
}
