/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 09:10:07 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/22 11:38:03 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int main(int ac, char **av)
{
  if (!ft_parse_args(ac, av))
    return (0);
  ft_launch_philo(ac, av);
}

ft_launch_philo(int ac, char **av)
{
  t_rules rules;
  t_barrier barrier;
  int i;

  ft_init_rules(&rules, av, ac, &barrier);
  ft_init_barrier(&barrier, rules.nb_philo);
  ft_init_philos(&rules);
  ft_start_simulation(&rules);
}

ft_dispatch(void *data)
{
  t_philo *philo;

  philo = (t_philo *)data;
  if ((philo->id % 2) == 0)
    ft_even_routine(philo);
  else
    ft_odd_routine(philo);
  return(0);
}

void ft_even_meal(t_philo *philo)
{
  pthread_mutex_lock(&(philo->rules->forks[philo->r_fork]));
  ft_print_message("has taken a fork", philo);
  pthread_mutex_lock(&(philo->rules->forks[philo->l_fork]));
  ft_print_message("has taken a fork", philo);
  pthread_mutex_lock(&(philo->rules->meal));
  ft_print_message("is eating", philo);
  philo->last_meal = ft_get_ms_time();
  pthread_mutex_unlock(&(philo->rules->meal));
  ft_usleep(philo->rules->time_to_eat);
  philo->nb_meals++;
  pthread_mutex_unlock(&(philo->rules->forks[philo->r_fork]));
  pthread_mutex_unlock(&(philo->rules->forks[philo->l_fork]));
}
void ft_odd_meal(t_philo *philo)
{
  pthread_mutex_lock(&(philo->rules->forks[philo->l_fork]));
  ft_print_message("has taken a fork", philo);
  pthread_mutex_lock(&(philo->rules->forks[philo->r_fork]));
  ft_print_message("has taken a fork", philo);
  pthread_mutex_lock(&(philo->rules->meal));
  ft_print_message("is eating", philo);
  philo->last_meal = ft_get_ms_time();
  pthread_mutex_unlock(&(philo->rules->meal));
  ft_usleep(philo->rules->time_to_eat);
  philo->nb_meals++;
  pthread_mutex_unlock(&(philo->rules->forks[philo->l_fork]));
  pthread_mutex_unlock(&(philo->rules->forks[philo->r_fork]));
}
void ft_even_routine(t_philo *philo)
{
  ft_barrier_wait(philo->rules->barrier);
  philo->last_meal = ft_get_ms_time();
  while (!philo->rules->anyone_dead)
  {
    ft_even_meal(philo);
    if (philo->nb_meals == philo->rules->max_meals)
    {
      ft_done_eating(philo->rules);
      break;
    }
    ft_print_message("is sleeping");
    ft_usleep(philo->rules->time_to_sleep);
    ft_print_message("is thinking");
  }
}

void ft_odd_routine(t_philo *philo)
{
  ft_barrier_wait(philo->rules->barrier);
  philo->last_meal = ft_get_ms_time();
  while (!philo->rules->anyone_dead)
  {
    ft_odd_meal(philo);
    if (philo->nb_meals == philo->rules->max_meals)
    {
      ft_done_eating(philo->rules);
      break;
    }
    ft_print_message("is sleeping");
    ft_usleep(philo->rules->time_to_sleep);
    ft_print_message("is thinking");
  }
}

ft_done_eating(t_rules *rules)
{
  pthread_mutex_lock(&rules->done_eating);
  rules->done++;
  pthread_mutex_unlock(&rules->done_eating);
}

void ft_start_simulation(t_rules *rules)
{
  int i;

  while (i < rules->nb_philo)
  {
    pthread_create(rules->philos->thread, NULL, ft_dispatch, &rules->philos[i]);
  }
}

void ft_init_philos(t_rules *rules)
{
  int i;

  i = 0;
  while (i < rules->nb_philo)
  {
    rules->philos[i].id = i + 1;
    rules->philos[i].rules = rules;
    rules->philos[i].r_fork = i;
    rules->philos[i].l_fork = i + 1;
    rules->philos[i].nb_meals = 0;
  }
}

ft_init_barrier(t_barrier *barrier, int nb_philo)
{
  barrier->counter = 0;
  barrier->total_threads = nb_philo + 1;
  pthread_mutex_init(&barrier->mutex, NULL);
}

ft_init_rules(t_rules *rules, char **av, int ac, t_barrier *barrier)
{
  rules->nb_philo = av[1];
  rules->time_to_die = av[2];
  rules->time_to_die = av[3];
  rules->time_to_sleep = av[4];
  if (ac == 6)
    rules->max_meals = av[5];
  else
    rules->max_meals = -1;
  rules->barrier = barrier;
  rules->anyone_dead = 0;
  ft_init_forks(rules, av[1]);
  pthread_mutex_init(&rules->print, NULL);
  pthread_mutex_init(&rules->meal, NULL);
}

ft_init_forks(t_rules *rules, int nbr_philo)
{
  int i;

  i = 0;
  while (i < nbr_philo)
  {
    pthread_mutex_init(&rules->forks[i], NULL);
    i++;
  }
}

void ft_death_watch(t_rules *rules)
{
  int i;

  while (!rules->anyone_dead && rules->done != rules->nb_philo)
  {
    i = 0;
    while (i < rules->nb_philo)
    {
      if ((ft_get_ms_time - rules->philos[i].last_meal) > rules->time_to_die)
      {
        rules->anyone_dead = 1;
        break;
      }
      i++;
    }
  }
  if (rules->done == rules->nb_philo)
    printf("All done eating");
}
