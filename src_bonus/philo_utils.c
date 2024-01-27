/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 11:14:38 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/27 17:07:04 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_error_exit(const char *error)
{
	printf("%s", error);
	exit(1);
}

void ft_sleep(long usec, t_philo *philo)
{
  long start_time;
  long elapsed_time;
  long remaining_time;
  start_time = ft_get_time(MICROSECOND);
  while (ft_get_time(MICROSECOND) - start_time <= usec)
  {
    if (ft_get_int(philo->philo_sem, &philo->end_simulation))
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

void	ft_starting_desynchro(t_philo *philo)
{
	if (philo->philo_nb % 2 == 0)
	{
		if (philo->id % 2 == 0)
			ft_sleep(3e4, philo);
	}
	else
	{
		if (philo->id % 2)
			ft_think(philo, 1);
	}
}

void	ft_write(t_states status, t_philo *philo)
{
	long	elapsed_time;

	elapsed_time = ft_get_time(MILLISECOND) - philo->start_time;
	if (philo->full || ft_get_int(philo->philo_sem, &philo->end_simulation))
		return ;
	sem_wait(philo->micro);
  if (ft_get_int(philo->philo_sem, &philo->end_simulation))
    return;
	else if (status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK)
		printf("%ld %d has taken a fork\n", elapsed_time, philo->id);
	else if (status == EATING)
		printf("%ld %d is eating\n", elapsed_time, philo->id);
	else if (status == SLEEPING)
		printf("%ld %d is sleeping\n", elapsed_time, philo->id);
	else if (status == THINKING)
		printf("%ld %d is thinking\n", elapsed_time, philo->id);
	else if (status == DIED)
  	printf("%ld %d died\n", elapsed_time, philo->id);
  if (status != DIED)
	  sem_post(philo->micro);
}
