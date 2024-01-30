/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:27:28 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/30 16:08:12 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_sem_init(t_table *table)
{
	sem_unlink(FORKS);
	sem_unlink(PRINT_SEM);
	sem_unlink(SYNC);
	sem_unlink(KILL_THEM_ALL);
	sem_unlink(START);
	table->micro = sem_open("print_semaphore", O_CREAT, 0644, 1);
	if (table->micro == SEM_FAILED)
		ft_error_exit("Erreur lors de la création du sémaphore micro\n");
	table->forks = sem_open(FORKS, O_CREAT, 0644, table->philo_nb);
	if (table->forks == SEM_FAILED)
		ft_error_exit("Erreur lors de la création du sémaphore forks\n");
	table->sync_sem = sem_open(SYNC, O_CREAT, 0644, 1);
	if (table->sync_sem == SEM_FAILED)
		ft_error_exit("Erreur lors de la création du sémaphore sync\n");
	table->kill_them_all = sem_open(KILL_THEM_ALL, O_CREAT, 0644, 0);
	if (table->kill_them_all == SEM_FAILED)
		ft_error_exit("Erreur lors de la création du sémaphore kill_them_all\n");
	table->start_sem = sem_open(START, O_CREAT, 0644, 0);
	if (table->sync_sem == SEM_FAILED)
		ft_error_exit("Erreur lors de la création du sémaphore start\n");
}

void	ft_init_philo(t_philo *philo, t_table *table)
{
	sem_unlink(PHILO_SEM);
	philo->start_time = table->start_time;
	philo->t_to_die = table->t_to_die;
	philo->t_to_eat = table->t_to_eat;
	philo->t_to_sleep = table->t_to_sleep;
	philo->micro = table->micro;
	philo->forks = table->forks;
	philo->philo_nb = table->philo_nb;
	philo->end_simulation = 0;
	philo->max_meals = table->max_meals;
	philo->meal_counter = 0;
	philo->full = 0;
	philo->id = table->philo_id;
	philo->start_sem = table->start_sem;
	philo->philo_sem = sem_open(PHILO_SEM, O_CREAT, 0644, 1);
	if (philo->philo_sem == SEM_FAILED)
		ft_error_exit("Erreur lors de la creation de la semaphore du philo\n");
	philo->sync_sem = table->sync_sem;
	philo->table = table;
}
