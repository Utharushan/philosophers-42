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

int main(int argc, char **argv)
{
    t_data  data;
    t_philo *philos;
    pthread_t *threads;
    int i;

    if (argc < 5 || argc > 6)
        return 1;
    if (init_data(&data, argc, argv))
        return 1;
    philos = malloc(sizeof(t_philo) * data.num_philos);
    threads = malloc(sizeof(pthread_t) * data.num_philos);
    if (!philos || !threads)
        return 1;

    i = 0;
    data.start_time = get_time();
    while (i < data.num_philos)
    {
        philos[i].id = i + 1;
        philos[i].meals_eaten = 0;
        philos[i].last_meal = data.start_time;
        philos[i].data = &data;
        philos[i].left_fork = &data.forks[i];
        philos[i].right_fork = &data.forks[(i + 1) % data.num_philos];
        i++;
    }

    // Create philosopher threads
    i = 0;
    while (i < data.num_philos)
    {
        if (pthread_create(&threads[i], NULL, philo_routine, &philos[i]) != 0)
            return 1;
        i++;
    }

    // Monitor for death or all eaten
    while (1)
    {
        i = 0;
        while (i < data.num_philos)
        {
            pthread_mutex_lock(&data.state_mutex);
            if (!data.died && (get_time() - philos[i].last_meal > data.time_to_die))
            {
                data.died = 1;
                pthread_mutex_unlock(&data.state_mutex);
                // print death
                pthread_mutex_lock(&data.print_mutex);
                {
                    char buf[64];
                    int len = 0;
                    long long timestamp = get_time() - data.start_time;
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
                    int idtmp = philos[i].id;
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
                    int j = 0;
                    while (msg[j])
                        buf[len++] = msg[j++];
                    buf[len++] = '\n';
                    write(1, buf, len);
                }
                pthread_mutex_unlock(&data.print_mutex);
                break;
            }
            pthread_mutex_unlock(&data.state_mutex);
            i++;
        }
        if (data.died || (data.must_eat > 0 && data.all_ate))
            break;
        usleep(1000);
    }

    // Join threads and clean up
    i = 0;
    while (i < data.num_philos)
    {
        pthread_join(threads[i], NULL);
        i++;
    }
    destroy(&data, philos);
    free(threads);
    return 0;
}
