/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 11:07:10 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/24 16:47:19 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void ft_parse_input(t_table *table, char **av)
{
  table->philo_nb = ft_atol(av[1]);
  table->t_to_die = ft_atol(av[2]) * 1e3;
  table->t_to_eat = ft_atol(av[3]) * 1e3;
  table->t_to_sleep = ft_atol(av[4]) * 1e3;
  if (table->t_to_die < 6e4 || table->t_to_eat < 6e4 || table->t_to_sleep < 6e4)
    ft_error_exit("Time to die, eat or sleep must be at least 60ms");
  if (av[5])
    table->max_meals = ft_atol(av[5]);
  else
    table->max_meals = -1;
}

void	ft_error_exit(const char *error)
{
	printf("%s", error);
	exit(1);
}