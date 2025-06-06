/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 07:56:54 by tuthayak          #+#    #+#             */
/*   Updated: 2025/06/06 07:56:54 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_data *data = philo->data;

    // Special case: 1 philosopher
    if (data->num_philos == 1)
    {
        pthread_mutex_lock(philo->left_fork);
        print_action(data, philo->id, "has taken a fork");
        ft_usleep(data->time_to_die, data);
        pthread_mutex_lock(&data->state_mutex);
        data->died = 1;
        pthread_mutex_unlock(&data->state_mutex);
        // print death message
        pthread_mutex_lock(&data->print_mutex);
        {
            char buf[64];
            int len = 0;
            long long timestamp = get_time() - data->start_time;
            if (timestamp == 0)
                buf[len++] = '0';
            else
            {
                char rev[32];
                int idx = 0;
                while (timestamp > 0)
                {
                    rev[idx++] = (timestamp % 10) + '0';
                    timestamp /= 10;
                }
                while (idx > 0)
                    buf[len++] = rev[--idx];
            }
            buf[len++] = ' ';
            int idtmp = philo->id;
            if (idtmp == 0)
                buf[len++] = '0';
            else
            {
                char rev2[16];
                int idx2 = 0;
                while (idtmp > 0)
                {
                    rev2[idx2++] = (idtmp % 10) + '0';
                    idtmp /= 10;
                }
                while (idx2 > 0)
                    buf[len++] = rev2[--idx2];
            }
            buf[len++] = ' ';
            char *msg = "died";
            int i = 0;
            while (msg[i])
                buf[len++] = msg[i++];
            buf[len++] = '\n';
            write(1, buf, len);
        }
        pthread_mutex_unlock(&data->print_mutex);
        pthread_mutex_unlock(philo->left_fork);
        return NULL;
    }

    // Small delay for even philosophers to avoid contention
    if (philo->id % 2 == 0)
        usleep(100);

    while (1)
    {
        pthread_mutex_lock(&data->state_mutex);
        if (data->died || (data->must_eat && data->all_ate))
        {
            pthread_mutex_unlock(&data->state_mutex);
            break;
        }
        pthread_mutex_unlock(&data->state_mutex);

        // Take forks (order depends on id parity)
        if (philo->id % 2 == 0)
        {
            pthread_mutex_lock(philo->right_fork);
            print_action(data, philo->id, "has taken a fork");
            pthread_mutex_lock(philo->left_fork);
            print_action(data, philo->id, "has taken a fork");
        }
        else
        {
            pthread_mutex_lock(philo->left_fork);
            print_action(data, philo->id, "has taken a fork");
            pthread_mutex_lock(philo->right_fork);
            print_action(data, philo->id, "has taken a fork");
        }

        // Eat
        pthread_mutex_lock(&data->state_mutex);
        philo->last_meal = get_time();
        philo->meals_eaten++;
        if (data->must_eat > 0 && philo->meals_eaten == data->must_eat)
            data->all_ate_count++;
        if (data->all_ate_count == data->num_philos)
            data->all_ate = 1;
        pthread_mutex_unlock(&data->state_mutex);

        print_action(data, philo->id, "is eating");
        ft_usleep(data->time_to_eat, data);

        // Release forks
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);

        // Sleep
        print_action(data, philo->id, "is sleeping");
        ft_usleep(data->time_to_sleep, data);

        // Think
        print_action(data, philo->id, "is thinking");

        // Check stop conditions again
        pthread_mutex_lock(&data->state_mutex);
        if (data->died || (data->must_eat && data->all_ate))
        {
            pthread_mutex_unlock(&data->state_mutex);
            break;
        }
        pthread_mutex_unlock(&data->state_mutex);
    }
    return NULL;
}
