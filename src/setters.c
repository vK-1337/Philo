/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 14:24:50 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/28 14:29:54 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_set_int(t_mtx *mtx, int *dest, int value)
{
	pthread_mutex_lock(mtx);
	*dest = value;
	pthread_mutex_unlock(mtx);
}

void	ft_set_long(t_mtx *mtx, long *dest, long value)
{
	pthread_mutex_lock(mtx);
	*dest = value;
	pthread_mutex_unlock(mtx);
}

void	ft_incr_int(t_mtx *mtx, int *dest)
{
	pthread_mutex_lock(mtx);
	(*dest)++;
	pthread_mutex_unlock(mtx);
}
