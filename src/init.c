/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 14:26:18 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/30 16:07:56 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_data_init(t_table *table)
{
	int	i;

	i = -1;
	table->all_ready = 0;
	table->end_simulation = 0;
	table->threads_running = 0;
	table->philos = ft_safe_malloc(table->philo_nb * sizeof(t_philo));
	table->forks = ft_safe_malloc(table->philo_nb * sizeof(t_fork));
	pthread_mutex_init(&table->table_mtx, NULL);
	pthread_mutex_init(&table->write_mtx, NULL);
	while (++i < table->philo_nb)
	{
		pthread_mutex_init(&table->forks[i].fork, NULL);
		table->forks[i].fork_id = i;
	}
	ft_philo_init(table);
}

void	ft_philo_init(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < table->philo_nb)
	{
		philo = table->philos + i;
		philo->id = i + 1;
		philo->meals_counter = 0;
		philo->full = 0;
		philo->table = table;
		pthread_mutex_init(&philo->philo_mtx, NULL);
		ft_assign_forks(philo, table->forks, i);
	}
}

void	ft_assign_forks(t_philo *philo, t_fork *forks, int philo_pos)
{
	int	philo_nb;

	philo_nb = philo->table->philo_nb;
	philo->first_fork = &forks[(philo_pos + 1) % philo_nb];
	philo->second_fork = &forks[philo_pos];
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &forks[philo_pos];
		philo->second_fork = &forks[(philo_pos + 1) % philo_nb];
	}
}
