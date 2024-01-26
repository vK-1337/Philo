/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 11:12:10 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/26 21:54:36 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

//      INCLUDES        //

# include <fcntl.h>
# include <pthread.h>
# include <semaphore.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>
#include <signal.h>

//        SEMAPHORES    //
# define PRINT_SEM "print_semaphore"
# define FORKS "/forks_semaphore"
# define SYNC "/sync_semaphore"
# define PHILO_SEM "/philo_semaphore"

//        TYPEDEF       //

typedef enum e_states
{
	THINKING,
	EATING,
	SLEEPING,
	DEAD,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIED
}			t_states;

typedef struct s_table
{
	int		philo_nb;
	long	t_to_die;
	long	t_to_eat;
	long	t_to_sleep;
	long	start_time;
	int		max_meals;
	sem_t	*micro;
	sem_t	*forks;
	sem_t	*sync_sem;
  int philo_id;
}			t_table;

typedef struct s_philo
{
	long	t_to_die;
	long	t_to_eat;
	long	t_to_sleep;
	long	last_meal;
	int		end_simulation;
	int max_meals;
  int meal_counter;
  int thread_rdy;
  int		philo_nb;
	int		full;
	int		id;
	long	start_time;
	sem_t	*forks;
	sem_t	*micro;
	sem_t	*philo_sem;
}			t_philo;

typedef enum e_time_code
{
	SECOND,
	MILLISECOND,
	MICROSECOND
}			t_time_code;

void	ft_philo(t_table *table);
void ft_dinner(t_table *table);
void *ft_routine(void *data);
void ft_eat(t_philo *philo);
void	ft_error_exit(const char *error);
void ft_sleep(long usec, t_philo *philo);
long ft_get_time(t_time_code time_code);
void	ft_starting_desynchro(t_philo *philo);
void	ft_write(t_states status, t_philo *philo);
int	ft_get_int(sem_t *semaphore, int *value);
long ft_get_long(sem_t *semaphore, long *value);
void ft_init_philo(t_philo *philo, t_table *table);
void ft_sem_init(t_table *table);
void	ft_parse_input(int ac, char **av, t_table *table);
void ft_set_long(sem_t *semaphore, long *dest, long value);
void ft_set_int(sem_t *semaphore, int *dest, int value);
int	ft_strlen(char *str);
int ft_is_space(const char c);
int ft_is_digit(const char c);
long ft_atol(const char *str);
const char *ft_valid_input(const char *str);
void	ft_reaper(t_table *table, t_philo *philo);
void	ft_think(t_philo *philo, int before_simulation);
#endif