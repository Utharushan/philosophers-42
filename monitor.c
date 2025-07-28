/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:30:00 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/28 17:30:00 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	simulation_should_end(t_data *data)
{
	return (data->died || (data->must_eat > 0 && data->all_ate));
}

int	monitor(t_data *data, t_philo *philos)
{
	int	sleep_time;

	sleep_time = 1000;
	if (data->num_philos > 50)
		sleep_time = 500;
	if (data->num_philos > 100)
		sleep_time = 250;
	while (1)
	{
		if (check_all_philos_death(data, philos))
			return (1);
		if (simulation_should_end(data))
			break ;
		usleep(sleep_time);
	}
	return (0);
}

void	join_and_cleanup(t_data *data, t_philo *philos, pthread_t *threads)
{
	int		i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	destroy(data, philos);
	free(threads);
}

int	main(int argc, char **argv)
{
	t_data		data;
	t_philo		*philos;
	pthread_t	*threads;

	if (argc < 5 || argc > 6)
	{
		write(2, "Error: Invalid number of arguments\n", 36);
		write(2, "Usage: ./philo number_of_philosophers time_to_die ", 51);
		write(2, "time_to_eat time_to_sleep ", 27);
		write(2, "[number_of_times_each_philosopher_must_eat]\n", 45);
		return (1);
	}
	if (init_data(&data, argc, argv))
		return (1);
	philos = malloc(sizeof(t_philo) * data.num_philos);
	threads = malloc(sizeof(pthread_t) * data.num_philos);
	if (!philos || !threads)
		return (1);
	data.start_time = get_time();
	init_philos(&data, philos);
	if (create_threads(&data, philos, threads))
		return (1);
	monitor(&data, philos);
	join_and_cleanup(&data, philos, threads);
	return (0);
}
