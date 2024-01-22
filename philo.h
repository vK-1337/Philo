/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 09:11:17 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/22 17:42:57 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_barrier	t_barrier;
typedef struct s_rules		t_rules;
typedef struct s_philo		t_philo;

typedef struct s_philo
{
	int						id;
	int						nb_meals;
	int						l_fork;
	int						r_fork;
	t_rules					*rules;
	pthread_t				thread;
	long long				last_meal;
	int						done_eating;
}							t_philo;

typedef struct s_rules
{
	int						done;
	int						nb_philo;
	int						max_meals;
	int						anyone_dead;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	t_barrier				*barrier;
	pthread_mutex_t			meal;
	pthread_mutex_t			print;
	t_philo					philos[250];
	pthread_mutex_t			forks[250];
	pthread_mutex_t			done_eating;
}							t_rules;

typedef struct s_barrier
{
	pthread_mutex_t			mutex;
	int						counter;
	int						total_threads;
}							t_barrier;

void						ft_usleep(long long time, t_rules *rules);
long long					ft_get_ms_time(void);
int							ft_atoi(const char *str);
int							ft_parse_args(int ac, char **av);
void						ft_launch_philo(int ac, char **av);
void						*ft_dispatch(void *data);
void						ft_print_message(char *message, t_philo *philo);
void						ft_barrier_wait(t_barrier *barrier);
void						ft_meal(t_philo *philo);
void						ft_done_eating(t_philo *philo);
void						ft_init_philos(t_rules *rules);
void						ft_init_barrier(t_barrier *barrier, int nb_philo);
void						ft_init_rules(t_rules *rules, char **av, int ac,
								t_barrier *barrier);
void						ft_start_simulation(t_rules *rules);
void						ft_routine(t_philo *philo);
void						ft_init_forks(t_rules *rules, int nbr_philo);
void						ft_death_watch(t_rules *rules);

#endif