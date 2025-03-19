/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:57:24 by tuthayak          #+#    #+#             */
/*   Updated: 2025/03/19 11:57:24 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup_simulation(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_philos)
		pthread_mutex_destroy(&data->forks[i]);
	pthread_mutex_destroy(&data->death_lock);
	pthread_mutex_destroy(&data->print_lock);
	free(data->forks);
	free(data->philos);
}
