/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:27:38 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/28 14:19:21 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_parse_input(char **av, t_table *table)
{
	table->philo_nb = ft_atol(av[1]);
	table->t_to_die = ft_atol(av[2]) * 1e3;
	table->t_to_eat = ft_atol(av[3]) * 1e3;
	table->t_to_sleep = ft_atol(av[4]) * 1e3;
	if (table->t_to_die < 6e4 || table->t_to_eat < 6e4
		|| table->t_to_sleep < 6e4)
		ft_error_exit("Time to die, eat or sleep must be at least 60ms");
	if (av[5])
		table->max_meals = ft_atol(av[5]);
	else
		table->max_meals = -1;
}
