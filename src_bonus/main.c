/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 11:13:21 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/27 18:29:12 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_table	table;

	if ((ac != 6) && (ac != 5))
		ft_error_exit("Wrong number of args");
	else
		ft_parse_input(ac, av, &table);
	ft_philo(&table);
	return (0);
}

void	ft_philo(t_table *table)
{
	int	i;
	int	pid;
  pid_t children[table->philo_nb];
  int status;
  int j;
  int error;
  pthread_t meal_checker;

	ft_sem_init(table);
  table->children = children;
	i = -1;
	sem_wait(table->sync_sem);
	while (++i < table->philo_nb)
	{
		pid = fork();
		if (pid == -1)
			ft_error_exit("Erreur lors de la creation du processus");
		if (pid == 0)
		{
			table->philo_id = i + 1;
			table->start_time = ft_get_time(MILLISECOND);
			ft_dinner(table);
			break ;
		}
    else
      children[i] = pid;
	}
	i = -1;
	while (++i < table->philo_nb * 2)
  {
		sem_wait(table->sync_sem);
  }
  i = -1;
  while (++i < table->philo_nb * 2)
		sem_post(table->start_sem);
	if (sem_wait(table->kill_them_all))
    ft_kill_them_all(table);
	sem_close(table->micro);
	sem_close(table->forks);
	sem_close(table->sync_sem);
  sem_close(table->kill_them_all);
	sem_unlink(FORKS);
	sem_unlink(PRINT_SEM);
	sem_unlink(SYNC);
  sem_unlink(KILL_THEM_ALL);
}
void ft_kill_them_all(t_table *table)
{
  int i;

  i = -1;
  while (++i < table->philo_nb)
  {
    kill(table->children[i], SIGKILL);
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

void	*ft_reaper(void *data)
{
	int	i;
  long	elapsed_time;
  t_philo *philo;

  philo = (t_philo *)data;
  sem_post(philo->sync_sem);
  sem_wait(philo->start_sem);
  ft_sleep(5000, philo);
	while (1)
	{
		if (ft_get_int(philo->philo_sem, &philo->full))
			break ;
    if (ft_get_time(MILLISECOND) - ft_get_long(philo->philo_sem, &philo->last_meal) > philo->table->t_to_die / 1e3)
    {
      if (ft_get_int(philo->philo_sem, &philo->end_simulation))
        break;
		  ft_write(DIED, philo);
      ft_set_int(philo->philo_sem, &philo->end_simulation, 1);
      sem_post(philo->table->kill_them_all);
      break;
    }
	}
  return (NULL);
}

void	ft_routine(void *data)
{
	t_philo	*philo;
	philo = (t_philo *)data;
  sem_post(philo->sync_sem);
  sem_wait(philo->start_sem);
  ft_set_int(philo->philo_sem, &philo->thread_rdy, 1);
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
	return;
}

void	ft_think(t_philo *philo, int before_simulation)
{
	long	t_to_eat;
	long	t_to_sleep;
	long	t_to_think;

	if (!before_simulation && !ft_get_int(philo->philo_sem, &philo->end_simulation))
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
      return;
	sem_wait(philo->forks);
	ft_write(TAKE_FIRST_FORK, philo);
	ft_write(TAKE_SECOND_FORK, philo);
	ft_set_long(philo->philo_sem, &philo->last_meal, ft_get_time(MILLISECOND));
	philo->meal_counter++;
  if (ft_get_int(philo->philo_sem, &philo->end_simulation))
      return;
	ft_write(EATING, philo);
	ft_sleep(philo->t_to_eat, philo);
	if (philo->max_meals > 0 && philo->meal_counter == philo->max_meals)
		ft_set_int(philo->philo_sem, &philo->full, 1);
	sem_post(philo->forks);
}
