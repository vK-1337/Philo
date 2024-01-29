/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 10:01:03 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/29 20:54:38 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_philo(t_table *table)
{
	// int			i;
	pid_t		children[200];
	pthread_t	meal_checker;

	ft_sem_init(table);
	table->children = children;
	sem_wait(table->sync_sem);
	ft_create_process(table);
	// i = -1;
	// while (++i < table->philo_nb * 2)
	// 	sem_wait(table->sync_sem);
	// i = -1;
	// while (++i <= table->philo_nb / 2)
		// sem_post(table->start_sem);
  pthread_create(&meal_checker, NULL, ft_meal_checker, table);
	sem_wait(table->kill_them_all);
	ft_kill_them_all(table);
	pthread_join(meal_checker, NULL);
	ft_clean_exit(table);
}

void	*ft_meal_checker(void *data)
{
	t_table	*table;
	int		status;
	int		i;

	table = (t_table *)data;
	i = -1;
	while (++i < table->philo_nb)
	{
		waitpid(-1, &status, 0);
		if (status == 256)
			break ;
	}
	sem_post(table->kill_them_all);
	return (NULL);
}

void	ft_solo_philo(t_table *table)
{
	pid_t		children[1];
	int			i;

	ft_sem_init(table);
	table->children = children;
	ft_create_process(table);
	i = -1;
	while (++i < table->philo_nb * 2)
		sem_wait(table->sync_sem);
	sem_post(table->start_sem);
  sem_post(table->start_sem);
	sem_wait(table->kill_them_all);
	ft_kill_them_all(table);
	ft_clean_exit(table);
}
