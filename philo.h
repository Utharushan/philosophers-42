/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:56:16 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/25 21:46:19 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_data
{
	int				num_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				must_eat;
	int				all_ate;
	int				died;
	int				all_ate_count;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	state_mutex;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal;
	t_data			*data;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				done_eating;
}	t_philo;

int			ft_atoi(const char *str);
long long	get_time(void);
void		ft_usleep(long long time, t_data *data);
void		print_action(t_data *data, int id, char *message);
void		timestamp_to_str(long long timestamp, char *buf, int *len);
void		id_to_str(int id, char *buf, int *len);
int			init_data(t_data *data, int argc, char **argv);
void		destroy(t_data *data, t_philo *philos);
void		*philo_routine(void *arg);
void		handle_one_philo(t_philo *philo);

#endif
