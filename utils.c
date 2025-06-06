/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 07:56:26 by tuthayak          #+#    #+#             */
/*   Updated: 2025/06/06 07:56:26 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int ft_atoi(const char *str)
{
    int i = 0;
    long res = 0;
    int sign = 1;

    if (!str || !*str)
        return -1;
    if (str[i] == '+' || str[i] == '-')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return -1;
        res = res * 10 + (str[i] - '0');
        if (res > 2147483647)
            return -1;
        i++;
    }
    res *= sign;
    if (res > 2147483647 || res < -2147483648)
        return -1;
    return (int)res;
}

long long get_time(void)
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL) < 0)
        return -1;
    return ((long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL);
}

void ft_usleep(long long time, t_data *data)
{
    long long start = get_time();
    while (!(data->died))
    {
        if (get_time() - start >= time)
            break;
        usleep(500);
    }
}

void print_action(t_data *data, int id, char *message)
{
    long long timestamp;
    char        buf[128];
    int         len;
    int         i;

    pthread_mutex_lock(&data->print_mutex);
    timestamp = get_time() - data->start_time;
    if (!data->died)
    {
        len = 0;
        // timestamp to string
        long long tmp = timestamp;
        if (tmp == 0)
            buf[len++] = '0';
        else
        {
            char rev[32];
            int idx = 0;
            while (tmp > 0)
            {
                rev[idx++] = (tmp % 10) + '0';
                tmp /= 10;
            }
            while (idx > 0)
                buf[len++] = rev[--idx];
        }
        buf[len++] = ' ';
        // id to string
        int idtmp = id;
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
        // message
        i = 0;
        while (message[i])
            buf[len++] = message[i++];
        buf[len++] = '\n';
        write(1, buf, len);
    }
    pthread_mutex_unlock(&data->print_mutex);
}
