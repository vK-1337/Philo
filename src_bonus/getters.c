/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 15:01:32 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/28 09:58:40 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_get_long(sem_t *semaphore, long *value)
{
	long	ret;

	sem_wait(semaphore);
	ret = *value;
	sem_post(semaphore);
	return (ret);
}

int	ft_get_int(sem_t *semaphore, int *value)
{
	int	ret;

	sem_wait(semaphore);
	ret = *value;
	sem_post(semaphore);
	return (ret);
}
