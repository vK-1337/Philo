/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 14:28:19 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/28 14:30:15 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_solo_dinner(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	ft_wait_all_threads(philo->table);
	ft_set_long(&philo->philo_mtx, &philo->last_meal, ft_get_time(MILLISECOND));
	ft_incr_int(&philo->table->table_mtx, &philo->table->threads_running);
	ft_write(TAKE_FIRST_FORK, philo);
	while (!simulation_finished(philo->table))
		usleep(42);
	return (NULL);
}

void	*ft_reaper(void *data)
{
	int		i;
	t_table	*table;

	table = (t_table *)data;
	while (!all_threads_running(&table->table_mtx, &table->threads_running,
			table->philo_nb))
		;
	while (!simulation_finished(table))
	{
		i = -1;
		while (++i < table->philo_nb && !simulation_finished(table))
		{
			if (ft_philo_dead(&table->philos[i]))
			{
				ft_set_int(&table->table_mtx, &table->end_simulation, 1);
				ft_write(DIED, &table->philos[i]);
			}
		}
		if (ft_all_ate(table))
			ft_set_int(&table->table_mtx, &table->end_simulation, 1);
	}
	return (NULL);
}

void	ft_wait_all_threads(t_table *table)
{
	while (!ft_get_int(&table->table_mtx, &table->all_ready))
		usleep(200);
}
