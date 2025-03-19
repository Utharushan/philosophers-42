/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:56:28 by tuthayak          #+#    #+#             */
/*   Updated: 2025/03/19 11:56:28 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: ./philo num_philos time_to_die time_to_eat ");
		printf("time_to_sleep [num_times_each_philo_must_eat]\n");
		return (1);
	}
	if (!parse_arguments(argc, argv, &data))
		return (1);
	if (!init_simulation(&data))
		return (1);
	start_simulation(&data);
	cleanup_simulation(&data);
	return (0);
}
