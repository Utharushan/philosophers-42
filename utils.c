/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 07:56:26 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/25 20:29:45 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	atoi_sign(const char *str, int *i)
{
	int		sign;

	sign = 1;
	if (str[*i] == '+' || str[*i] == '-')
	{
		if (str[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

static int	atoi_convert(const char *str, int i, int sign)
{
	long	res;

	res = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		res = res * 10 + (str[i] - '0');
		if (res > 2147483647)
			return (-1);
		i++;
	}
	res *= sign;
	if (res > 2147483647 || res < -2147483648)
		return (-1);
	return ((int)res);
}

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;

	i = 0;
	if (!str || !*str)
		return (-1);
	sign = atoi_sign(str, &i);
	return (atoi_convert(str, i, sign));
}

long long	get_time(void)
{
	struct timeval		tv;

	if (gettimeofday(&tv, NULL) < 0)
		return (-1);
	return ((long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL);
}

void	ft_usleep(long long time, t_data *data)
{
	long long	start;
	int		stop;

	start = get_time();
	stop = 0;
	while (!stop)
	{
		pthread_mutex_lock(&data->state_mutex);
		if (data->died || (data->must_eat > 0 && data->all_ate))
			stop = 1;
		pthread_mutex_unlock(&data->state_mutex);
		if (get_time() - start >= time)
			break ;
		usleep(data->num_philos > 100 ? 500 : 100); // Less contention for large N
	}
}

void	print_action(t_data *data, int id, char *message)
{
	long long	timestamp;
	char		buf[128];
	int			len;
	int			i;

	pthread_mutex_lock(&data->print_mutex);
	timestamp = get_time() - data->start_time;
	if (!data->died)
	{
		len = 0;
		timestamp_to_str(timestamp, buf, &len);
		buf[len++] = ' ';
		id_to_str(id, buf, &len);
		buf[len++] = ' ';
		i = 0;
		while (message[i])
			buf[len++] = message[i++];
		buf[len++] = '\n';
		write(1, buf, len);
	}
	pthread_mutex_unlock(&data->print_mutex);
}
