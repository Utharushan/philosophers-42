/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 07:57:10 by tuthayak          #+#    #+#             */
/*   Updated: 2025/06/06 07:57:10 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_philos(t_data *data, t_philo *philos)
{
	int		i;

	i = 0;
	while (i < data->num_philos)
	{
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = data->start_time;
		philos[i].data = data;
		philos[i].left_fork = &data->forks[i];
		philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		philos[i].done_eating = 0;
		i++;
	}
	return (0);
}

static int	create_threads(t_data *data, t_philo *philos, pthread_t *threads)
{
	int		i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&threads[i], NULL, philo_routine, &philos[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

static void	print_death(t_data *data, t_philo *philo)
{
	char		buf[64];
	int			len;
	long long	timestamp;
	char		*msg;
	int			j;

	len = 0;
	timestamp = get_time() - data->start_time;
	timestamp_to_str(timestamp, buf, &len);
	buf[len++] = ' ';
	id_to_str(philo->id, buf, &len);
	buf[len++] = ' ';
	msg = "died";
	j = 0;
	while (msg[j])
		buf[len++] = msg[j++];
	buf[len++] = '\n';
	write(1, buf, len);
}

static int	monitor(t_data *data, t_philo *philos)
{
	int		i;

	while (1)
	{
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_lock(&data->state_mutex);
			if (!data->died && (get_time() - philos[i].last_meal > data->time_to_die))
			{
				data->died = 1;
				pthread_mutex_unlock(&data->state_mutex);
				pthread_mutex_lock(&data->print_mutex);
				print_death(data, &philos[i]);
				pthread_mutex_unlock(&data->print_mutex);
				return (1);
			}
			pthread_mutex_unlock(&data->state_mutex);
			i++;
		}
		if (data->died || (data->must_eat > 0 && data->all_ate))
			break;
		usleep(500); // 500 microseconds for more precise death detection
	}
	return (0);
}

static void	join_and_cleanup(t_data *data, t_philo *philos, pthread_t *threads)
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
		return (1);
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
