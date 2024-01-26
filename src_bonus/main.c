/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 11:13:21 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/26 22:10:23 by vda-conc         ###   ########.fr       */
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

	ft_sem_init(table);
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
	while (++i < table->philo_nb)
		sem_post(table->sync_sem);
	i = -1;
  int status;
  int j;
	while (++i < table->philo_nb)
	{
		wait(&status);
    if (status == 256)
    {
      j = -1;
      while (++j < table->philo_nb)
      {
        kill(children[j], SIGINT);
      }
    }
    else if (status == 0)
      printf("HE ATE GOOD BRO\n");
	}
	sem_close(table->micro);
	sem_close(table->forks);
	sem_close(table->sync_sem);
	sem_unlink(PHILO_SEM);
	sem_unlink(FORKS);
	sem_unlink(PRINT_SEM);
	sem_unlink(SYNC);
}

void	ft_dinner(t_table *table)
{
	pthread_t	philo_thread;
	t_philo		philo;
	int			error;

	ft_init_philo(&philo, table);
	error = pthread_create(&philo_thread, NULL, ft_routine, &philo);
	if (error)
		ft_error_exit("Error during thread creation");
	ft_reaper(table, &philo);
	pthread_join(philo_thread, NULL);
  printf("HE DIED MAN\n");
	sem_close(philo.philo_sem);
  if (philo.full == 1)
     exit(0);
  else
    exit(1);
}

void	ft_reaper(t_table *table, t_philo *philo)
{
	int	i;
  long	elapsed_time;

  ft_sleep(2000, philo);
	while (1)
	{
		if (ft_get_int(philo->philo_sem, &philo->full))
			break ;
    if (ft_get_time(MILLISECOND) - ft_get_long(philo->philo_sem, &philo->last_meal) > table->t_to_die / 1e3)
    {
      sem_wait(table->micro);
      ft_set_int(philo->philo_sem, &philo->end_simulation, 1);
      elapsed_time = ft_get_time(MILLISECOND) - philo->start_time;
		  printf("%ld %d died\n", elapsed_time, philo->id);
      break;
    }
	}
}

void	*ft_routine(void *data)
{
	t_philo	*philo;
	philo = (t_philo *)data;
  ft_set_int(philo->philo_sem, &philo->thread_rdy, 1);
  ft_set_long(philo->philo_sem, &philo->last_meal, ft_get_time(MILLISECOND));
  ft_starting_desynchro(philo);
	while (!philo->end_simulation)
	{
		ft_eat(philo);
		if (ft_get_int(philo->philo_sem, &philo->full))
			break ;
    ft_write(SLEEPING, philo);
		ft_sleep(philo->t_to_sleep, philo);
    ft_think(philo, 0);
	}
	return (NULL);
}

void	ft_think(t_philo *philo, int before_simulation)
{
	long	t_to_eat;
	long	t_to_sleep;
	long	t_to_think;

	if (!before_simulation)
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
	sem_wait(philo->forks);
	ft_write(TAKE_FIRST_FORK, philo);
	ft_write(TAKE_SECOND_FORK, philo);
	ft_set_long(philo->philo_sem, &philo->last_meal, ft_get_time(MILLISECOND));
	philo->meal_counter++;
	ft_write(EATING, philo);
	ft_sleep(philo->t_to_eat, philo);
	if (philo->max_meals > 0 && philo->meal_counter == philo->max_meals)
		ft_set_int(philo->philo_sem, &philo->full, 1);
	sem_post(philo->forks);
}
