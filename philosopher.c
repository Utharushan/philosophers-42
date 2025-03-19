/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:56:56 by tuthayak          #+#    #+#             */
/*   Updated: 2025/03/19 11:56:56 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	assign_forks(t_philo *philo, t_data *data, int i)
{
	philo->left_fork = &data->forks[i];
	if (i == data->num_philos - 1)
		philo->right_fork = &data->forks[0];
	else
		philo->right_fork = &data->forks[i + 1];
}

void	init_philosophers(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].eat_count = 0;
		data->philos[i].last_meal_time = get_time_in_ms();
		data->philos[i].data = data;
		assign_forks(&data->philos[i], data, i);
	}
}
