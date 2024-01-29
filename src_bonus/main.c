/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 11:13:21 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/29 11:02:58 by vda-conc         ###   ########.fr       */
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
	ft_philo(&table);
	return (0);
}
// ! NEED TO HANDLE THE SOLO PHILO CASE
// ! NEED TO HANDLE ERRORS CREATING THREADS
// ! JUST LET THE FIRST HALF EAT AND THEN SEM POST IN EACH LOOP SO WHEN FIRST FINISHED EATING, FIRST OF 2ND HALF CAN EAT