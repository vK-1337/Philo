/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 11:07:29 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/30 16:06:30 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

//      INCLUDES        //

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

//        TYPEDEF       //

typedef pthread_mutex_t	t_mtx;
typedef struct s_table	t_table;
typedef struct s_fork	t_fork;

typedef struct s_philo
{
	int					id;
	long				meals_counter;
	long				last_meal;
	int					full;
	t_fork				*first_fork;
	t_fork				*second_fork;
	t_table				*table;
	pthread_t			thread;
	t_mtx				philo_mtx;
}						t_philo;

typedef enum e_time_code
{
	SECOND,
	MILLISECOND,
	MICROSECOND
}						t_time_code;

struct					s_table
{
	long				start_time;
	int					philo_nb;
	int					max_meals;
	long				t_to_die;
	long				t_to_eat;
	long				t_to_sleep;
	t_mtx				table_mtx;
	int					all_ready;
	int					end_simulation;
	t_philo				*philos;
	t_fork				*forks;
	t_mtx				write_mtx;
	int					threads_running;
	pthread_t			reaper;
};

struct					s_fork
{
	int					fork_id;
	t_mtx				fork;
};

typedef enum e_states
{
	THINKING,
	EATING,
	SLEEPING,
	DEAD,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIED
}						t_states;

//       MAIN FUNCTION      //
void					ft_philo(t_table *table);

//     PARSING UTILS     //
void					ft_parse_input(t_table *table, char **av);
int						ft_is_space(const char c);
int						ft_is_digit(const char c);
const char				*ft_valid_input(const char *str);
int						ft_strlen(char *str);
long					ft_atol(const char *str);

//      ACTIONS       //
void					ft_write(t_states status, t_philo *philo);
void					ft_eat(t_philo *philo);
void					ft_sleep(long usec, t_table *table);
void					ft_think(t_philo *philo, int before_simulation);

//      INIT       //
void					ft_data_init(t_table *table);
void					ft_philo_init(t_table *table);
void					*ft_safe_malloc(size_t bytes);

//      SETTERS       //
void					ft_set_long(t_mtx *mtx, long *dest, long value);
void					ft_set_int(t_mtx *mtx, int *dest, int value);
void					ft_incr_int(t_mtx *mtx, int *dest);

//      GETTERS     //
long					get_long(t_mtx *mtx, long *value);
int						ft_get_int(t_mtx *mtx, int *value);
long					ft_get_time(t_time_code time_code);
void					ft_assign_forks(t_philo *philo, t_fork *forks,
							int philo_pos);

//      THREADS     //
void					ft_wait_all_threads(t_table *table);
void					*ft_solo_dinner(void *data);
void					*ft_reaper(void *data);
int						all_threads_running(t_mtx *mutex, int *threads,
							int philo_nbr);
void					ft_starting_desynchro(t_philo *philo);
int						ft_philo_dead(t_philo *philo);

//      DINNER UTILS     //
void					ft_start_dinner(t_table *table);
void					*ft_dinner(void *data);
int						ft_all_ate(t_table *table);
int						simulation_finished(t_table *table);
void					ft_finish_dinner(t_table *table);

// THREAD ERROR HANDLE //
void					ft_thread_error_handle(int error);
void					ft_error_exit(const char *error);
#endif
