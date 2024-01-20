/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 16:52:02 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/20 16:52:31 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int ft_atoi(const char *str)
{
  int i;
  int sign;
  int nb;

  i = 0;
  sign = 1;
  nb = 0;
  if (!str)
    return (0);
  while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || \
      str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
    i++;
  if (str[i] == '-')
    sign = -sign;
  if (str[i] == '-' || str[i] == '+')
    i++;
  if (str[i] < 48 || str[i] > 57)
    return (0);
  while (str[i] >= 48 && str[i] <= 57)
    nb = nb * 10 + (str[i++] - 48);
  return (nb * sign);
}

int check_args(int argc, char **argv)
{
  int i;

  i = 1;
  while (i < argc)
  {
    if (ft_atoi(argv[i]) < 0)
      return (0);
    i++;
  }
  return (1);
}