/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vk <vk@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 10:01:40 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/28 21:32:51 by vk               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	sem_post(philo->sync_sem);
	sem_wait(philo->start_sem);
  philo->start_time = ft_get_time(MILLISECOND);
	ft_set_long(philo->philo_sem, &philo->last_meal, ft_get_time(MILLISECOND));
	ft_starting_desynchro(philo);
	while (!philo->end_simulation)
	{
		ft_eat(philo);
		if (ft_get_int(philo->philo_sem, &philo->full))
			return ;
		ft_write(SLEEPING, philo);
		ft_sleep(philo->t_to_sleep, philo);
		ft_think(philo, 0);
	}
	return ;
}

void	*ft_reaper(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	sem_post(philo->sync_sem);
	sem_wait(philo->start_sem);
	ft_sleep(5000, philo);
	while (1)
	{
		if (ft_get_int(philo->philo_sem, &philo->full))
			break ;
		if (ft_get_time(MILLISECOND) - ft_get_long(philo->philo_sem,
				&philo->last_meal) > philo->table->t_to_die / 1e3)
		{
			ft_write(DIED, philo);
			ft_set_int(philo->philo_sem, &philo->end_simulation, 1);
			sem_post(philo->table->kill_them_all);
			break ;
		}
	}
	return (NULL);
}

void	ft_create_process(t_table *table)
{
	int	i;
	int	pid;

	i = -1;
	table->start_time = ft_get_time(MILLISECOND);
	while (++i < table->philo_nb)
	{
		pid = fork();
		if (pid == -1)
			ft_error_exit("Erreur lors de la creation du processus");
		if (pid == 0)
		{
			table->philo_id = i + 1;
			ft_dinner(table);
			exit(0);
		}
		else
			table->children[i] = pid;
	}
}

void	ft_dinner(t_table *table)
{
	pthread_t	reaper_thread;
	t_philo		philo;
	int			error;

	ft_init_philo(&philo, table);
	error = pthread_create(&reaper_thread, NULL, ft_reaper, &philo);
	if (error)
		ft_error_exit("Error during thread creation");
	pthread_detach(reaper_thread);
	ft_routine(&philo);
	sem_close(philo.philo_sem);
	sem_unlink(PHILO_SEM);
	if (philo.full == 1)
		exit(0);
	else
		exit(1);
}
