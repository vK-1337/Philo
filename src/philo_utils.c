/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 15:14:44 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/28 14:29:49 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	ft_all_ate(t_table *table)
{
	int	i;
	int	counter;

	i = -1;
	counter = 0;
	while (++i < table->philo_nb)
	{
		if (ft_get_int(&table->table_mtx, &table->philos[i].full))
			counter++;
	}
	if (counter == ft_get_int(&table->table_mtx, &table->philo_nb))
		return (1);
	return (0);
}

int	all_threads_running(t_mtx *mutex, int *threads, int philo_nbr)
{
	int	ret;

	ret = 0;
	pthread_mutex_lock(mutex);
	if (*threads == philo_nbr)
		ret = 1;
	pthread_mutex_unlock(mutex);
	return (ret);
}

void	*ft_dinner(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	ft_wait_all_threads(philo->table);
	ft_set_long(&philo->philo_mtx, &philo->last_meal, ft_get_time(MILLISECOND));
	ft_incr_int(&philo->table->table_mtx, &philo->table->threads_running);
	ft_starting_desynchro(philo);
	while (!simulation_finished(philo->table))
	{
		if (philo->full)
			break ;
		ft_eat(philo);
		ft_write(SLEEPING, philo);
		ft_sleep(philo->table->t_to_sleep, philo->table);
		ft_think(philo, 0);
	}
	return (NULL);
}

void	ft_starting_desynchro(t_philo *philo)
{
	if (philo->table->philo_nb % 2 == 0)
	{
		if (philo->id % 2 == 0)
			ft_sleep(3e4, philo->table);
	}
	else
	{
		if (philo->id % 2)
			ft_think(philo, 1);
	}
}
