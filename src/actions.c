/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 14:23:50 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/28 14:25:50 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_write(t_states status, t_philo *philo)
{
	long	elapsed_time;

	elapsed_time = ft_get_time(MILLISECOND) - philo->table->start_time;
	if (philo->full)
		return ;
	pthread_mutex_lock(&philo->table->write_mtx);
	if ((status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK)
		&& !simulation_finished(philo->table))
		printf("%ld %d has taken a fork\n", elapsed_time, philo->id);
	else if (status == EATING && !simulation_finished(philo->table))
		printf("%ld %d is eating\n", elapsed_time, philo->id);
	else if (status == SLEEPING && !simulation_finished(philo->table))
		printf("%ld %d is sleeping\n", elapsed_time, philo->id);
	else if (status == THINKING && !simulation_finished(philo->table))
		printf("%ld %d is thinking\n", elapsed_time, philo->id);
	else if (status == DIED)
		printf("%ld %d died\n", elapsed_time, philo->id);
	pthread_mutex_unlock(&philo->table->write_mtx);
}

void	ft_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->first_fork->fork);
	ft_write(TAKE_FIRST_FORK, philo);
	pthread_mutex_lock(&philo->second_fork->fork);
	ft_write(TAKE_SECOND_FORK, philo);
	ft_set_long(&philo->philo_mtx, &philo->last_meal, ft_get_time(MILLISECOND));
	philo->meals_counter++;
	ft_write(EATING, philo);
	ft_sleep(philo->table->t_to_eat, philo->table);
	if (philo->table->max_meals > 0
		&& philo->meals_counter == philo->table->max_meals)
		ft_set_int(&philo->philo_mtx, &philo->full, 1);
	pthread_mutex_unlock(&philo->first_fork->fork);
	pthread_mutex_unlock(&philo->second_fork->fork);
}

void	ft_sleep(long usec, t_table *table)
{
	long	start_time;
	long	elapsed_time;
	long	remaining_time;

	start_time = ft_get_time(MICROSECOND);
	while (ft_get_time(MICROSECOND) - start_time <= usec)
	{
		if (ft_get_int(&table->table_mtx, &table->end_simulation))
			break ;
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

void	ft_think(t_philo *philo, int before_simulation)
{
	long	t_to_eat;
	long	t_to_sleep;
	long	t_to_think;

	if (!before_simulation)
		ft_write(THINKING, philo);
	if (philo->table->philo_nb % 2 == 0)
		return ;
	t_to_eat = philo->table->t_to_eat;
	t_to_sleep = philo->table->t_to_sleep;
	t_to_think = t_to_eat * 2 - t_to_sleep;
	if (t_to_think < 0)
		t_to_think = 0;
	ft_sleep(t_to_think * 0.42, philo->table);
}
