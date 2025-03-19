/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:56:16 by tuthayak          #+#    #+#             */
/*   Updated: 2025/03/19 11:56:16 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

// Other includes
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

// Struct Definitions (Ensure they are properly defined)
typedef struct s_philo
{
	int				id;
	long long		last_meal_time;
	int				eat_count;
	struct s_data	*data;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_t		thread;
}	t_philo;

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_meals;
	int				someone_died;
	int				full_count;
	long long		start_time;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}	t_data;

// Parsing & Initialization
int			parse_arguments(int argc, char **argv, t_data *data);
int			init_simulation(t_data *data);
void		cleanup_simulation(t_data *data);

// Simulation
void		*philosopher_routine(void *arg);
void		start_simulation(t_data *data);
void		*check_death(void *arg);

// Time & Utils
long long	get_time_in_ms(void);
void		ft_usleep(int time);
void		print_action(t_philo *philo, char *msg);

#endif
