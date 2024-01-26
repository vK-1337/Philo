/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:57:42 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/26 17:07:09 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void ft_set_long(sem_t *semaphore, long *dest, long value)
{

  sem_wait(semaphore);
  *dest = value;
  sem_post(semaphore);
}
void ft_set_int(sem_t *semaphore, int *dest, int value)
{
  sem_wait(semaphore);
  *dest = value;
  sem_post(semaphore);
}
