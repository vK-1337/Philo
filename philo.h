/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 10:36:51 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/20 17:39:28 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_death		t_death;
typedef struct s_philo		t_philo;
typedef struct s_barrier	t_barrier;
typedef struct s_times		t_times;
typedef struct s_micro		t_micro;

typedef struct s_barrier
{
	pthread_mutex_t			mutex;
	int						counter;
	int						total_threads;
}							t_barrier;

typedef struct s_philo
{
	pthread_mutex_t			*r_fork;
	pthread_mutex_t			*l_fork;
	int						r_fork_index;
	int						l_fork_index;
	int						id;
	pthread_mutex_t			*print_mutex;
	pthread_t				*philos;
	pthread_mutex_t			*forks;
	t_barrier				*barrier;
	int						number_of_philo;
	int						*forks_data;
	long long				time_to_die;
	long long				time_to_eat;
	long long				time_to_sleep;
	long long				last_meal;
	int						max_meals;
	int						nb_eat;
	int						even;
	t_death					*death;
}							t_philo;

typedef struct s_death
{
	pthread_mutex_t			*forks;
	int						*forks_data;
	pthread_mutex_t			*print_mutex;
	long long				time_to_die;
	int						number_of_philo;
	t_philo					*philos_data_arr;
	pthread_t				*philos;
	t_barrier				*barrier;
	int						anyone_dead;
}							t_death;

typedef struct s_times
{
	long long				time_to_die;
	long long				time_to_eat;
	long long				time_to_sleep;
	long long				max_meals;
	t_barrier				*barrier;
}							t_times;

typedef struct s_micro
{
	pthread_mutex_t			print_mutex;
  pthread_mutex_t     odd_print_mutex;
}							t_micro;

# define NC "\e[0m"
# define YELLOW "\e[1;33m"
# define BLUE "\e[1;34m"
# define GREEN "\e[1;32m"

long long					ft_get_ms_time(void);
void						ft_usleep(long long time);
void						ft_print_message(char *message, t_philo *philo);
void						*ft_odd_thread(t_philo *philo,
								long long time_to_eat, long long time_to_sleep);
void						*ft_even_thread(t_philo *philo,
								long long time_to_eat, long long time_to_sleep);
void						ft_barrier_wait(t_barrier *barrier);
int							ft_has_done_eating(t_philo *philo);
int							ft_atoi(const char *str);
int							check_args(int argc, char **argv);

#endif