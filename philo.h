/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 10:36:51 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/18 20:12:45 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*print_mutex;
	int				even;
}					t_philo;

typedef struct s_micro
{
	pthread_mutex_t	print_mutex;
}					t_micro;

# define NC "\e[0m"
# define YELLOW "\e[1;33m"
# define BLUE "\e[1;34m"
# define GREEN "\e[1;32m"

long long			ft_get_ms_time(void);
void				ft_usleep(long long time);
void				ft_print_message(char *message, pthread_t tid,
						t_philo *philo);
void				ft_odd_thread(pthread_t tid, t_philo *philo);
void				ft_even_thread(pthread_t tid, t_philo *philo);
#endif