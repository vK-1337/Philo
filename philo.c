/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 10:36:26 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/19 18:58:10 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_routine(void *data)
{
	pthread_t	tid;
	t_philo		*philo;
	int			i;

	i = 1;
	tid = pthread_self();
	philo = (t_philo *)data;
	philo->nb_eat = 0;
	ft_barrier_wait(philo->barrier);
	while (philo->nb_eat < philo->max_meals || philo->max_meals == -1)
	{
		if (philo->even)
			ft_even_thread(tid, philo, philo->time_to_eat,
				philo->time_to_sleep);
		else
			ft_odd_thread(tid, philo, philo->time_to_eat, philo->time_to_sleep);
		if (philo->death->anyone_dead)
			return (NULL);
	}
	ft_print_message("Finished eating", tid, philo);
	return (NULL);
}

void	*ft_even_thread(pthread_t tid, t_philo *philo, long long time_to_eat,
		long long time_to_sleep)
{
	philo->last_meal = ft_get_ms_time();
	ft_print_message("is thinking", tid, philo);
	pthread_mutex_lock(philo->r_fork);
	if (philo->death->anyone_dead)
		return (pthread_mutex_unlock(philo->r_fork), NULL);
	ft_print_message("has taken a fork", tid, philo);
	pthread_mutex_lock(philo->l_fork);
	if (philo->death->anyone_dead)
		return (pthread_mutex_unlock(philo->r_fork),
			pthread_mutex_unlock(philo->r_fork), NULL);
	ft_print_message("has taken a fork", tid, philo);
	if (philo->death->anyone_dead)
		return (pthread_mutex_unlock(philo->r_fork),
			pthread_mutex_unlock(philo->r_fork), NULL);
	ft_print_message("is eating", tid, philo);
	philo->last_meal = ft_get_ms_time();
	ft_usleep(time_to_eat);
	philo->nb_eat++;
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
	if (philo->death->anyone_dead)
		return (NULL);
	ft_print_message("is sleeping", tid, philo);
	ft_usleep(time_to_sleep);
	if (philo->death->anyone_dead)
		return (NULL);
	return (NULL);
}

void	*ft_odd_thread(pthread_t tid, t_philo *philo, long long time_to_eat,
		long long time_to_sleep)
{
	philo->last_meal = ft_get_ms_time();
	ft_print_message("is thinking", tid, philo);
	if (philo->death->anyone_dead)
		return (NULL);
	pthread_mutex_lock(philo->l_fork);
	if (philo->death->anyone_dead)
		return (pthread_mutex_unlock(philo->l_fork), NULL);
	ft_print_message("has taken a fork", tid, philo);
	if (philo->death->anyone_dead)
		return (pthread_mutex_unlock(philo->l_fork), NULL);
	pthread_mutex_lock(philo->r_fork);
	if (philo->death->anyone_dead)
		return (pthread_mutex_unlock(philo->r_fork),
			pthread_mutex_unlock(philo->l_fork), NULL);
	ft_print_message("has taken a fork", tid, philo);
	if (philo->death->anyone_dead)
		return (pthread_mutex_unlock(philo->r_fork),
			pthread_mutex_unlock(philo->l_fork), NULL);
	ft_print_message("is eating", tid, philo);
	philo->last_meal = ft_get_ms_time();
	ft_usleep(time_to_eat);
	philo->nb_eat++;
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
	if (philo->death->anyone_dead)
		return (NULL);
	ft_print_message("is sleeping", tid, philo);
	ft_usleep(time_to_sleep);
	if (philo->death->anyone_dead)
		return (NULL);
	return (NULL);
}

int	is_dead(t_philo *philo, pthread_t tid)
{
	int	i;
	int	j;

	if (philo->last_meal > 0 && ft_get_ms_time()
		- philo->last_meal > philo->time_to_die)
	{
		pthread_mutex_lock(philo->print_mutex);
		printf("%s|%lld|%s %s|%lu|%s %s%s%s\n", GREEN, ft_get_ms_time(), NC,
			YELLOW, tid, NC, BLUE, "died", NC);
		i = 0;
		philo->death->anyone_dead = 1;
		j = 0;
		while (j++ < philo->number_of_philo)
			pthread_join(philo->philos[j], NULL);
		i = philo->number_of_philo;
		while (i--)
			pthread_mutex_destroy(&philo->forks[i]);
		pthread_mutex_unlock(philo->print_mutex);
		pthread_mutex_destroy(philo->print_mutex);
		return (-1);
	}
	else
		return (0);
}

void	ft_usleep(long long time)
{
	long long	start;

	start = ft_get_ms_time();
	while (ft_get_ms_time() - start < time)
		usleep(1);
}

void	ft_print_message(char *message, pthread_t tid, t_philo *philo)
{
	pthread_mutex_lock(philo->print_mutex);
	printf("%s|%lld|%s %s|%lu|%s %s%s%s\n", GREEN, ft_get_ms_time(), NC, YELLOW,
		tid, NC, BLUE, message, NC);
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

void	*ft_solo_philo(void *data)
{
	t_philo		*philo;
	pthread_t	tid;

	tid = pthread_self();
	philo = (t_philo *)data;
	ft_barrier_wait(philo->barrier);
	philo->last_meal = ft_get_ms_time();
	philo->nb_eat = 0;
	while ((philo->nb_eat < philo->max_meals || philo->max_meals == -1))
	{
		if (philo->death->anyone_dead)
			return (NULL);
		ft_print_message("is thinking", tid, philo);
		pthread_mutex_lock(philo->r_fork);
		if (philo->death->anyone_dead)
			return (pthread_mutex_unlock(philo->r_fork), NULL);
		ft_print_message("has taken a fork", tid, philo);
		pthread_mutex_lock(philo->l_fork);
		if (philo->death->anyone_dead)
			return (pthread_mutex_unlock(philo->r_fork),
				pthread_mutex_unlock(philo->l_fork), NULL);
		ft_print_message("has taken a fork", tid, philo);
		if (philo->death->anyone_dead)
			return (pthread_mutex_unlock(philo->r_fork),
				pthread_mutex_unlock(philo->l_fork), NULL);
		ft_print_message("is eating", tid, philo);
		philo->last_meal = ft_get_ms_time();
		ft_usleep(philo->time_to_eat);
		philo->nb_eat++;
		pthread_mutex_unlock(philo->r_fork);
		if (philo->death->anyone_dead)
			return (NULL);
		ft_print_message("is sleeping", tid, philo);
		ft_usleep(philo->time_to_sleep);
		if (philo->death->anyone_dead)
			return (NULL);
	}
	return (NULL);
}

void	*ft_death_watch(void *data)
{
	t_death		*death;
	pthread_t	tid;
	int			i;

	int should_break ;
	tid = pthread_self();
	should_break = 0;
	death = (t_death *)data;
	ft_barrier_wait(death->barrier);
	while (1)
	{
		i = 0;
		while (i < death->number_of_philo)
		{
			if (is_dead(&death->philos_data_arr[i], death->philos[i]) == -1)
				return (NULL);
			i++;
		}
	}
	return (NULL);
}

int	ft_create_philo(int number, t_times *times)
{
	t_philo			philo_data[number];
	pthread_t		philos[number];
	pthread_mutex_t	forks[number];
	pthread_t		death_watcher;
	t_death			death_watcher_data;
	t_micro			micro;
	int				i;

	i = 0;
	pthread_mutex_init(&micro.print_mutex, NULL);
	death_watcher_data.forks = forks;
	death_watcher_data.print_mutex = &micro.print_mutex;
	death_watcher_data.time_to_die = times->time_to_die;
	death_watcher_data.number_of_philo = number;
	death_watcher_data.philos_data_arr = philo_data;
	death_watcher_data.philos = philos;
	death_watcher_data.barrier = times->barrier;
	while (i < number)
	{
		pthread_mutex_init(&forks[i], NULL);
		philo_data[i].print_mutex = &micro.print_mutex;
		philo_data[i].r_fork = &forks[i];
		philo_data[i].time_to_die = times->time_to_die;
		philo_data[i].time_to_eat = times->time_to_eat;
		philo_data[i].time_to_sleep = times->time_to_sleep;
		philo_data[i].max_meals = times->max_meals;
		philo_data[i].last_meal = 0;
		philo_data[i].number_of_philo = number;
		philo_data[i].philos = philos;
		philo_data[i].forks = forks;
		philo_data[i].barrier = times->barrier;
		philo_data[i].death = &death_watcher_data;
		if (i % 2)
			philo_data[i].even = 1;
		else
			philo_data[i].even = 0;
		if (i + 1 == number)
			philo_data[i].l_fork = &forks[0];
		else
			philo_data[i].l_fork = &forks[i + 1];
		if (number == 1)
			pthread_create(&philos[i], NULL, ft_solo_philo, &philo_data[i]);
		else
			pthread_create(&philos[i], NULL, ft_routine, &philo_data[i]);
		printf("Philosopher %d created\n", i + 1);
		i++;
	}
	printf("Death watcher created\n");
	pthread_create(&death_watcher, NULL, ft_death_watch, &death_watcher_data);
	while (i--)
		pthread_join(philos[i], NULL);
	pthread_join(death_watcher, NULL);
	return (0);
}

int	main(int ac, char **av)
{
	t_times		times;
	t_barrier	barrier;

	if (ac < 5 || ac > 6)
	{
		printf("Usage : ./philo [nbr_of_philos] [time_to_die] [time_to_eat] [time_to_sleep]\n");
		printf("Example : ./philo 5 800 200 200\n");
		printf("You can also add a fifth argument to specify the number of times each philosopher must eat\n");
		printf("Example : ./philo 5 800 200 200 7\n");
		return (0);
	}
	pthread_mutex_init(&barrier.mutex, NULL);
	barrier.counter = 0;
	barrier.total_threads = ft_atoi(av[1]) + 1;
	times.time_to_die = ft_atoi(av[2]);
	times.time_to_eat = ft_atoi(av[3]);
	times.time_to_sleep = ft_atoi(av[4]);
	times.barrier = &barrier;
	if (ac == 6)
		times.max_meals = ft_atoi(av[5]);
	else
		times.max_meals = -1;
	ft_create_philo(ft_atoi(av[1]), &times);
	printf("All philosophers have finished eating\n");
	pthread_mutex_destroy(&barrier.mutex);
	return (0);
}

void	ft_barrier_wait(t_barrier *barrier)
{
	pthread_mutex_lock(&barrier->mutex);
	barrier->counter++;
	pthread_mutex_unlock(&barrier->mutex);
	while (barrier->counter < barrier->total_threads)
		usleep(100);
}