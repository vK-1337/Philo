/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 11:48:56 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/28 14:18:35 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_table	dinner_table;

	if ((ac != 6) && (ac != 5))
		ft_error_exit("Wrong number of arguments");
	else
		ft_parse_input(&dinner_table, av);
	ft_philo(&dinner_table);
}
