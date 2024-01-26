/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 15:14:44 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/25 11:48:49 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void ft_sleep(long usec, t_table *table)
{
  long start_time;
  long elapsed_time;
  long remaining_time;
  start_time = ft_get_time(MICROSECOND);
  while (ft_get_time(MICROSECOND) - start_time <= usec)
  {
    if (ft_get_int(&table->table_mtx, &table->end_simulation))
      break;
    elapsed_time = ft_get_time(MICROSECOND) - start_time;
    remaining_time = usec - elapsed_time;
    if (remaining_time > 1e3)
      usleep(remaining_time / 2);
    else
    {
      while (ft_get_time(MICROSECOND) - start_time <= usec)
        ;
    }
  }
}

long ft_get_time(t_time_code time_code)
{
  struct timeval tv;

  if(gettimeofday(&tv, NULL))
    ft_error_exit("gettimeofday failed");
  if (time_code == SECOND)
    return (tv.tv_sec + (tv.tv_usec / 1e6));
  else if (time_code == MILLISECOND)
    return (tv.tv_sec * 1e3 + (tv.tv_usec / 1e3));
  else if (time_code == MICROSECOND)
    return (tv.tv_sec * 1e6 + tv.tv_usec);
  else
    ft_error_exit("Wrong time_code");
  return (1337);
}

void ft_set_int(t_mtx *mtx, int *dest, int value)
{
  pthread_mutex_lock(mtx);
  *dest = value;
  pthread_mutex_unlock(mtx);
}

void ft_set_long(t_mtx *mtx, long *dest, long value)
{
  pthread_mutex_lock(mtx);
  *dest = value;
  pthread_mutex_unlock(mtx);
}

void ft_incr_int(t_mtx *mtx, int *dest)
{
  pthread_mutex_lock(mtx);
  (*dest)++;
  pthread_mutex_unlock(mtx);
}