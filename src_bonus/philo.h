/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 11:12:10 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/30 16:08:20 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

//      INCLUDES        //

# include <fcntl.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

//        SEMAPHORES    //
# define PRINT_SEM "print_semaphore"
# define FORKS "/forks_semaphore"
# define SYNC "/sync_semaphore"
# define PHILO_SEM "/philo_semaphore"
# define KILL_THEM_ALL "/all_dead_semaphore"
# define START "/start_semaphore"

//        TYPEDEF       //

typedef enum e_states
{
	THINKING,
	EATING,
	SLEEPING,
	DEAD,
	TAKE_FORK,
	DIED
}			t_states;

typedef struct s_table
{
	int		philo_nb;
	int		philo_id;
	int		max_meals;
	long	t_to_die;
	long	t_to_eat;
	long	t_to_sleep;
	long	start_time;
	sem_t	*micro;
	sem_t	*forks;
	sem_t	*sync_sem;
	pid_t	*children;
	sem_t	*kill_them_all;
	sem_t	*start_sem;
}			t_table;

typedef struct s_philo
{
	long	t_to_die;
	long	t_to_eat;
	long	t_to_sleep;
	long	last_meal;
	int		end_simulation;
	int		max_meals;
	int		meal_counter;
	int		thread_rdy;
	int		philo_nb;
	int		full;
	int		id;
	long	start_time;
	sem_t	*forks;
	sem_t	*micro;
	sem_t	*philo_sem;
	sem_t	*sync_sem;
	sem_t	*start_sem;
	t_table	*table;
}			t_philo;

typedef enum e_time_code
{
	SECOND,
	MILLISECOND,
	MICROSECOND
}			t_time_code;

//        PROTOTYPES      //

// MAIN //
void		ft_philo(t_table *table);
void		ft_solo_philo(t_table *table);

// DINNER //
void		ft_routine(void *data);
void		ft_dinner(t_table *table);

// DEATH //
void		*ft_reaper(void *data);
void		ft_kill_them_all(t_table *table);

// UTILS //
int			ft_strlen(char *str);
long		ft_atol(const char *str);
int			ft_is_space(const char c);
int			ft_is_digit(const char c);
void		ft_clean_exit(t_table *table);
void		ft_error_exit(const char *error);
long		ft_get_time(t_time_code time_code);
void		ft_starting_desynchro(t_philo *philo);
const char	*ft_valid_input(const char *str);

// INIT //
void		ft_sem_init(t_table *table);
void		ft_create_process(t_table *table);
void		ft_init_philo(t_philo *philo, t_table *table);
void		ft_parse_input(char **av, t_table *table);

// GETTERS //
int			ft_get_int(sem_t *semaphore, int *value);
long		ft_get_long(sem_t *semaphore, long *value);

// SETTERS //
void		ft_set_int(sem_t *semaphore, int *dest, int value);
void		ft_set_long(sem_t *semaphore, long *dest, long value);

// ACTIONS //
void		ft_eat(t_philo *philo);
void		*ft_meal_checker(void *data);
void		ft_sleep(long usec, t_philo *philo);
void		ft_write(t_states status, t_philo *philo);
void		ft_think(t_philo *philo, int before_simulation);

// ERRORS HANDLE //
void		ft_thread_error_handle(int error);

#endif
