/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 14:48:28 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/30 16:06:16 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_philo(t_table *table)
{
	ft_data_init(table);
	ft_start_dinner(table);
	ft_finish_dinner(table);
}

void	ft_finish_dinner(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->philo_nb)
		pthread_mutex_destroy(&table->forks[i].fork);
	pthread_mutex_destroy(&table->table_mtx);
	pthread_mutex_destroy(&table->write_mtx);
	free(table->philos);
	free(table->forks);
}

void	ft_start_dinner(t_table *table)
{
	int	i;

	i = -1;
	if (table->max_meals == 0)
		return ;
	else if (table->philo_nb == 1)
		ft_thread_error_handle(pthread_create(&table->philos[0].thread, NULL,
				ft_solo_dinner, &table->philos[0]));
	else
	{
		while (++i < table->philo_nb)
			ft_thread_error_handle(pthread_create(&table->philos[i].thread,
					NULL, ft_dinner, &table->philos[i]));
	}
	ft_thread_error_handle(pthread_create(&table->reaper, NULL, ft_reaper,
			table));
	table->start_time = ft_get_time(MILLISECOND);
	ft_set_int(&table->table_mtx, &table->all_ready, 1);
	i = -1;
	while (++i < table->philo_nb)
		pthread_join(table->philos[i].thread, NULL);
	pthread_join(table->reaper, NULL);
}

int	ft_philo_dead(t_philo *philo)
{
	long	elapsed_time;
	long	t_to_die;

	if (ft_get_int(&philo->philo_mtx, &philo->full) == 1)
		return (0);
	elapsed_time = ft_get_time(MILLISECOND) - get_long(&philo->philo_mtx,
			&philo->last_meal);
	t_to_die = philo->table->t_to_die / 1000;
	if (elapsed_time > t_to_die)
		return (1);
	return (0);
}

void	*ft_safe_malloc(size_t bytes)
{
	void	*ptr;

	ptr = malloc(bytes);
	if (!ptr)
		ft_error_exit("Error: malloc failed\n");
	return (ptr);
}
