/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 09:26:52 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/22 15:27:25 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_usleep(long long time)
{
	long long	start;

	start = ft_get_ms_time();
	while (ft_get_ms_time() - start < time)
		usleep(1);
}

long long	ft_get_ms_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	if (gettimeofday(&tv, NULL) == 0)
		return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
	else
	{
		printf("Error while getting time\n");
		return (-1);
	}
}

void	ft_barrier_wait(t_barrier *barrier)
{
	pthread_mutex_lock(&barrier->mutex);
	barrier->counter++;
	pthread_mutex_unlock(&barrier->mutex);
	while (barrier->counter < barrier->total_threads)
		usleep(10);
}

void	ft_print_message(char *message, t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->print);
	printf("%lld %d %s\n", ft_get_ms_time(), philo->id, message);
	pthread_mutex_unlock(&philo->rules->print);
}