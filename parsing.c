/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 15:39:27 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/20 15:42:45 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		ft_atoi(const char *str)
{
  int		i;
  int		neg;
  int		nb;

  i = 0;
  neg = 1;
  nb = 0;
  while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || \
      str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
    i++;
  if (str[i] == '-')
    neg = -1;
  while (str[i] == '+' || str[i] == '-')
    i++;
  if (str[i] < '0' || str[i] > '9')
    return (-1);
  while (str[i] >= '0' && str[i] <= '9' && str[i])
  {
    nb = nb * 10 + str[i] - 48;
    i++;
  }
  return (nb * neg);
}

