/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 09:10:07 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/22 16:36:46 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int main(int ac, char **av)
{
  if (!ft_parse_args(ac, av))
    return (0);
  ft_launch_philo(ac, av);
}

void ft_launch_philo(int ac, char **av)
{
  t_rules rules;
  t_barrier barrier;

  ft_init_rules(&rules, av, ac, &barrier);
  ft_init_barrier(&barrier, rules.nb_philo);
  ft_init_philos(&rules);
  ft_start_simulation(&rules);
}

void *ft_dispatch(void *data)
{
  t_philo *philo;

  philo = (t_philo *)data;
  if ((philo->id % 2) == 0)
    ft_even_routine(philo);
  else
    ft_odd_routine(philo);
  return (NULL);
}

void ft_even_meal(t_philo *philo)
{
  t_rules *rules;
  rules = philo->rules;
  pthread_mutex_lock(&(rules->forks[philo->r_fork]));
  ft_print_message("has taken a fork", philo);
  pthread_mutex_lock(&(rules->forks[philo->l_fork]));
  ft_print_message("has taken a fork", philo);
  pthread_mutex_lock(&(rules->meal));
  ft_print_message("is eating", philo);
  philo->last_meal = ft_get_ms_time();
  pthread_mutex_unlock(&(rules->meal));
  ft_usleep(philo->rules->time_to_eat, philo->rules);
  philo->nb_meals++;
  pthread_mutex_unlock(&(philo->rules->forks[philo->r_fork]));
  pthread_mutex_unlock(&(philo->rules->forks[philo->l_fork]));
}
void ft_odd_meal(t_philo *philo)
{
  t_rules *rules;

  rules = philo->rules;
  pthread_mutex_lock(&(rules->forks[philo->l_fork]));
  ft_print_message("has taken a fork", philo);
  pthread_mutex_lock(&(rules->forks[philo->r_fork]));
  ft_print_message("has taken a fork", philo);
  pthread_mutex_lock(&(rules->meal));
  ft_print_message("is eating", philo);
  philo->last_meal = ft_get_ms_time();
  pthread_mutex_unlock(&(rules->meal));
  philo->nb_meals++;
  pthread_mutex_unlock(&(rules->forks[philo->l_fork]));
  pthread_mutex_unlock(&(rules->forks[philo->r_fork]));
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
      ft_done_eating(philo);
      break;
    }
    ft_print_message("is sleeping", philo);
    ft_usleep(philo->rules->time_to_sleep, philo->rules);
    ft_print_message("is thinking", philo);
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
      ft_done_eating(philo);
      break;
    }
    ft_print_message("is sleeping", philo);
    ft_usleep(philo->rules->time_to_sleep, philo->rules);
    ft_print_message("is thinking", philo);
  }
}

void ft_done_eating(t_philo *philo)
{
  pthread_mutex_lock(&philo->rules->done_eating);
  philo->rules->done++;
  philo->done_eating = 1;
  pthread_mutex_unlock(&philo->rules->done_eating);
}

void ft_start_simulation(t_rules *rules)
{
  int i;

  i = 0;
  while (i < rules->nb_philo)
  {
    pthread_create(&rules->philos->thread, NULL, ft_dispatch, &rules->philos[i]);
    i++;
  }
  ft_death_watch(rules);
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
    if (i + 1 == rules->nb_philo)
    {
      rules->philos[i].l_fork = 0;
    }
    else
      rules->philos[i].l_fork = i + 1;
    rules->philos[i].nb_meals = 0;
    rules->philos[i].done_eating = 0;
    i++;
  }
}

void ft_init_barrier(t_barrier *barrier, int nb_philo)
{
  barrier->counter = 0;
  barrier->total_threads = nb_philo + 1;
  pthread_mutex_init(&barrier->mutex, NULL);
}

void ft_init_rules(t_rules *rules, char **av, int ac, t_barrier *barrier)
{
  rules->nb_philo = ft_atoi(av[1]);
  rules->time_to_die = ft_atoi(av[2]);
  rules->time_to_eat = ft_atoi(av[3]);
  rules->time_to_sleep = ft_atoi(av[4]);
  if (ac == 6)
    rules->max_meals = ft_atoi(av[5]);
  else
    rules->max_meals = -1;
  rules->barrier = barrier;
  rules->anyone_dead = 0;
  ft_init_forks(rules, ft_atoi(av[1]));
  pthread_mutex_init(&rules->print, NULL);
  pthread_mutex_init(&rules->meal, NULL);
}

void ft_init_forks(t_rules *rules, int nbr_philo)
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

  ft_barrier_wait(rules->barrier);
  ft_usleep(1000, rules);
  while (!rules->anyone_dead && rules->done != rules->nb_philo)
  {
    i = 0;
    while (i < rules->nb_philo)
    {
      if (((ft_get_ms_time() - rules->philos[i].last_meal) > rules->time_to_die) && !(rules->philos[i].done_eating))
      {
        ft_print_message("died", &rules->philos[i]);
        rules->anyone_dead = 1;
        break;
      }
      i++;
    }
  }
  if (rules->done == rules->nb_philo)
    printf("All done eating");
}
