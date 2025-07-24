/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 07:56:41 by tuthayak          #+#    #+#             */
/*   Updated: 2025/06/06 07:56:41 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_data_values(t_data *data, int argc, char **argv)
{
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->must_eat = -1;
	data->died = 0;
	data->all_ate = 0;
	data->all_ate_count = 0;
	if (data->num_philos <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0)
		return (1);
	if (argc == 6)
	{
		data->must_eat = ft_atoi(argv[5]);
		if (data->must_eat <= 0)
			return (1);
	}
	else
		data->must_eat = 0;
	return (0);
}

static int	init_mutexes(t_data *data)
{
	int			i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->state_mutex, NULL) != 0)
		return (1);
	return (0);
}

int	init_data(t_data *data, int argc, char **argv)
{
	if (init_data_values(data, argc, argv))
		return (1);
	if (init_mutexes(data))
		return (1);
	return (0);
}

void	destroy(t_data *data, t_philo *philos)
{
	int		i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->state_mutex);
	free(data->forks);
	free(philos);
}
