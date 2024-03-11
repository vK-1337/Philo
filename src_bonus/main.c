/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 11:13:21 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/29 19:29:39 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_table	table;

	if ((ac != 6) && (ac != 5))
		ft_error_exit("Wrong number of args");
	else
		ft_parse_input(av, &table);
	if (table.philo_nb == 1)
		ft_solo_philo(&table);
	else
		ft_philo(&table);
	return (0);
}
