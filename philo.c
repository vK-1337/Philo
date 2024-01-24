/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vk <vk@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 14:48:28 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/24 22:59:55 by vk               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_philo(t_table *table)
{
	ft_data_init(table);
	ft_start_dinner(table);
  ft_finish_dinner(table);
}

void ft_finish_dinner(t_table *table)
{
  int i;

  i = -1;
  while (++i < table->philo_nb)
  {
    pthread_mutex_destroy(&table->philos[i].philo_mtx);
    pthread_mutex_destroy(&table->forks[i].fork);
  }
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
		pthread_create(&table->philos[0].thread, NULL, ft_solo_dinner, &table->philos[0]);
	else
	{
		while (++i < table->philo_nb)
			pthread_create(&table->philos[i].thread, NULL, ft_dinner,
				&table->philos[i]);
	}
  pthread_create(&table->reaper, NULL, ft_reaper, table);
	table->start_time = ft_get_time(MILLISECOND);
	ft_set_int(&table->table_mtx, &table->all_ready, 1);
	i = -1;
	while (++i < table->philo_nb)
		pthread_join(table->philos[i].thread, NULL);
  ft_set_int(&table->table_mtx, &table->end_simulation, 1);
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

void *ft_solo_dinner(void *data)
{
  t_philo *philo;

  philo = (t_philo*)data;
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
      if (ft_philo_dead(&table->philos[i]))
      {
        ft_set_int(&table->table_mtx, &table->end_simulation, 1);
        ft_write(DIED, &table->philos[i]);
      }
  }
  return (NULL);
}

int	all_threads_running(t_mtx *mutex, int *threads, int philo_nbr)
{
	int	ret;

	ret = 0;
	pthread_mutex_lock(mutex);
	if (*threads == philo_nbr)
		ret = 1;
	pthread_mutex_unlock(mutex);
	return (ret);
}
void	*ft_dinner(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	ft_wait_all_threads(philo->table);
  ft_set_long(&philo->philo_mtx, &philo->last_meal, ft_get_time(MILLISECOND));
	ft_incr_int(&philo->table->table_mtx, &philo->table->threads_running);
	while (!simulation_finished(philo->table))
	{
		if (philo->full)
			break ;
		ft_eat(philo);
		ft_write(SLEEPING, philo);
		ft_sleep(philo->table->t_to_sleep, philo->table);
		ft_think(philo);
	}
	return (NULL);
}

void	ft_think(t_philo *philo)
{
	ft_write(THINKING, philo);
}

void	ft_write(t_states status, t_philo *philo)
{
	long	elapsed_time;

	elapsed_time = ft_get_time(MILLISECOND) - philo->table->start_time;
	if (philo->full)
		return ;
	pthread_mutex_lock(&philo->table->write_mtx);
	if ((status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK)
		&& !simulation_finished(philo->table))
		printf("%ld %d has taken a fork\n", elapsed_time, philo->id);
	else if (status == EATING && !simulation_finished(philo->table))
		printf("%ld %d is eating\n", elapsed_time, philo->id);
	else if (status == SLEEPING && !simulation_finished(philo->table))
		printf("%ld %d is sleeping\n", elapsed_time, philo->id);
	else if (status == THINKING && !simulation_finished(philo->table))
		printf("%ld %d is thinking\n", elapsed_time, philo->id);
	else if (status == DIED)
		printf("%ld %d died\n", elapsed_time, philo->id);
	pthread_mutex_unlock(&philo->table->write_mtx);
}

void	ft_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->first_fork->fork);
	ft_write(TAKE_FIRST_FORK, philo);
	pthread_mutex_lock(&philo->second_fork->fork);
	ft_write(TAKE_SECOND_FORK, philo);
	ft_set_long(&philo->philo_mtx, &philo->last_meal, ft_get_time(MILLISECOND));
	philo->meals_counter++;
	ft_write(EATING, philo);
	ft_sleep(philo->table->t_to_eat, philo->table);
	if (philo->table->max_meals > 0
		&& philo->meals_counter == philo->table->max_meals)
		ft_set_int(&philo->philo_mtx, &philo->full, 1);
	pthread_mutex_unlock(&philo->first_fork->fork);
	pthread_mutex_unlock(&philo->second_fork->fork);
}

void	ft_wait_all_threads(t_table *table)
{
	while (!ft_get_int(&table->table_mtx, &table->all_ready))
		;
}

int	ft_get_int(t_mtx *mtx, int *value)
{
	int	ret;

	pthread_mutex_lock(mtx);
	ret = *value;
	pthread_mutex_unlock(mtx);
	return (ret);
}

int	simulation_finished(t_table *table)
{
	return (ft_get_int(&table->table_mtx, &table->end_simulation));
}

long	get_long(t_mtx *mtx, long *value)
{
	long	ret;

	pthread_mutex_lock(mtx);
	ret = *value;
	pthread_mutex_unlock(mtx);
	return (ret);
}
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

void	*ft_safe_malloc(size_t bytes)
{
	void *ptr;

	ptr = malloc(bytes);
	if (!ptr)
		ft_error_exit("Error: malloc failed\n");
	return (ptr);
}
