/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 10:36:26 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/18 20:14:37 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_routine(void *data)
{
	pthread_t	tid;
	t_philo		*philo;
  int    i;

  i = 1;
	tid = pthread_self();
	philo = (t_philo *)data;
	while (i)
	{
    if (philo->even)
      ft_even_thread(tid, philo);
    else
      ft_odd_thread(tid, philo);
    i++;
	}
	return (NULL);
}

void ft_even_thread(pthread_t tid, t_philo *philo)
{
    ft_print_message("Started thinking", tid, philo);
    ft_print_message("Trying to take the r_fork", tid, philo);
    pthread_mutex_lock(philo->r_fork);
    ft_print_message("Took the r_fork", tid, philo);
    ft_print_message("Trying to take the l_fork", tid, philo);
    pthread_mutex_lock(philo->l_fork);
    ft_print_message("Took the l_fork", tid, philo);
    ft_print_message("Started eating", tid, philo);
    ft_usleep(2);
    pthread_mutex_unlock(philo->r_fork);
    ft_print_message("Dropped the r_fork", tid, philo);
    pthread_mutex_unlock(philo->l_fork);
    ft_print_message("Dropped the l_fork", tid, philo);
    ft_print_message("Started sleeping", tid, philo);
    ft_usleep(2);
}

void ft_odd_thread(pthread_t tid, t_philo *philo)
{
    ft_print_message("Started thinking", tid, philo);
    ft_print_message("Trying to take the l_fork", tid, philo);
    pthread_mutex_lock(philo->l_fork);
    ft_print_message("Took the l_fork", tid, philo);
    ft_print_message("Trying to take the r_fork", tid, philo);
    pthread_mutex_lock(philo->r_fork);
    ft_print_message("Took the r_fork", tid, philo);
    ft_print_message("Started eating", tid, philo);
    ft_usleep(2);
    ft_print_message("Trying to drop the l_fork", tid, philo);
    pthread_mutex_unlock(philo->l_fork);
    ft_print_message("Dropped the l_fork", tid, philo);
    ft_print_message("Trying to drop the r_fork", tid, philo);
    pthread_mutex_unlock(philo->r_fork);
    ft_print_message("Dropped the r_fork", tid, philo);
    ft_print_message("Started sleeping", tid, philo);
    ft_usleep(2);
}

void ft_usleep(long long time)
{
  long long start;

  start = ft_get_ms_time();
  while (ft_get_ms_time() - start < time)
    usleep(100);
}

void ft_print_message(char *message, pthread_t tid, t_philo *philo)
{
  pthread_mutex_lock(philo->print_mutex);
  printf("At => %s|%lld|%s Thread => %s|%lu|%s %s%s%s\n", GREEN, ft_get_ms_time(), NC, YELLOW, tid, NC, BLUE, message, NC);
  pthread_mutex_unlock(philo->print_mutex);
}

long long	ft_get_ms_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	if (gettimeofday(&tv, NULL) == 0)
		return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
	else
	{
		printf("Erreur lors de l'obtention du temps\n");
		return (-1);
	}
}

int	ft_create_philo(int number)
{
	t_philo		philo[number];
	pthread_t	philos[number];
  pthread_mutex_t forks[number];
  t_micro micro;
	int			i;

	i = 0;
  pthread_mutex_init(&micro.print_mutex, NULL);
	while (i < number)
	{
    pthread_mutex_init(&forks[i], NULL);
    philo[i].print_mutex = &micro.print_mutex;
    philo[i].r_fork = &forks[i];
    if (i % 2)
      philo[i].even = 1;
    else
      philo[i].even = 0;
    if (i + 1 == number)
      philo[i].l_fork = &forks[0];
    else
      philo[i].l_fork = &forks[i + 1];
		pthread_create(&philos[i], NULL, ft_routine, &philo[i]);
    printf("Philosopher %d created\n", i + 1);
    i++;
	}
	pthread_join(philos[0], NULL);
  pthread_join(philos[1], NULL);
	return (0);
}

int	main(int ac, char **av)
{
	(void)ac;
  (void)av;
	ft_create_philo(av[1][0] - '0');
}