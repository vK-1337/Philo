/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 09:11:17 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/22 11:23:29 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NEW_PHILO_H
# define NEW_PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_rules
{
	int				done;
	int				nb_philo;
	int				max_meals;
	int				anyone_dead;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	t_barrier		*barrier;
	pthread_mutex_t	meal;
	pthread_mutex_t	print;
	t_philo			philos[250];
	pthread_mutex_t	forks[250];
	pthread_mutex_t	done_eating;
}					t_rules;

typedef struct s_philo
{
	int				id;
	int				nb_meals;
	int				l_fork;
	int				r_fork;
	t_rules			*rules;
	pthread_t		thread;
	long long		last_meal;
}					t_philo;

typedef struct s_barrier
{
	pthread_mutex_t	mutex;
	int				counter;
	int				total_threads;
}					t_barrier;

void				ft_usleep(long long time);
long long			ft_get_ms_time(void);
int					ft_atoi(const char *str);
int					ft_parse_args(int ac, char **av);

#endif