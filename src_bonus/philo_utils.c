/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 11:14:38 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/29 20:38:59 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_error_exit(const char *error)
{
	printf("%s", error);
	exit(1);
}

long	ft_get_time(t_time_code time_code)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
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
  if (philo->id % 2 == 0)
    ft_sleep(2e4, philo);
}

void	ft_clean_exit(t_table *table)
{
	sem_close(table->micro);
	sem_close(table->forks);
	sem_close(table->sync_sem);
	sem_close(table->kill_them_all);
	sem_close(table->start_sem);
	sem_unlink(FORKS);
	sem_unlink(PRINT_SEM);
	sem_unlink(SYNC);
	sem_unlink(KILL_THEM_ALL);
	sem_unlink(START);
}
