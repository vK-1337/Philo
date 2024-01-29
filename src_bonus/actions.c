/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 10:00:17 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/29 19:29:23 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_sleep(long usec, t_philo *philo)
{
	long	start_time;
	long	elapsed_time;
	long	remaining_time;

	start_time = ft_get_time(MICROSECOND);
	while (ft_get_time(MICROSECOND) - start_time < usec)
	{
		if (ft_get_int(philo->philo_sem, &philo->end_simulation))
			break ;
		elapsed_time = ft_get_time(MICROSECOND) - start_time;
		remaining_time = usec - elapsed_time;
		if (remaining_time > 1e3)
			usleep(remaining_time / 2);
		else
		{
			while (ft_get_time(MICROSECOND) - start_time < usec)
				;
		}
	}
}

void	ft_think(t_philo *philo, int before_simulation)
{
	long	t_to_eat;
	long	t_to_sleep;
	long	t_to_think;

	if (!before_simulation && !ft_get_int(philo->philo_sem,
			&philo->end_simulation))
		ft_write(THINKING, philo);
	if (philo->philo_nb % 2 == 0)
		return ;
	t_to_eat = philo->t_to_eat;
	t_to_sleep = philo->t_to_sleep;
	t_to_think = t_to_eat * 2 - t_to_sleep;
	if (t_to_think < 0)
		t_to_think = 0;
	ft_sleep(t_to_think * 0.50, philo);
}

void	ft_eat(t_philo *philo)
{
	if (ft_get_int(philo->philo_sem, &philo->end_simulation))
		return ;
	sem_wait(philo->forks);
	ft_write(TAKE_FORK, philo);
	sem_wait(philo->forks);
	ft_write(TAKE_FORK, philo);
	ft_set_long(philo->philo_sem, &philo->last_meal, ft_get_time(MILLISECOND));
	ft_write(EATING, philo);
	ft_sleep(philo->t_to_eat, philo);
	sem_post(philo->forks);
	sem_post(philo->forks);
	philo->meal_counter++;
	if (ft_get_int(philo->philo_sem, &philo->end_simulation))
		return ;
	if (philo->max_meals > 0 && philo->meal_counter == philo->max_meals)
		ft_set_int(philo->philo_sem, &philo->full, 1);
}

void	ft_write(t_states status, t_philo *philo)
{
	long	elapsed_time;

	if (philo->full || ft_get_int(philo->philo_sem, &philo->end_simulation))
		return ;
	sem_wait(philo->micro);
	elapsed_time = ft_get_time(MILLISECOND) - philo->start_time;
	if (ft_get_int(philo->philo_sem, &philo->end_simulation))
		return ;
	else if (status == TAKE_FORK)
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

void	ft_kill_them_all(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->philo_nb)
		kill(table->children[i], SIGKILL);
}
