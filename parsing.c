/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 16:52:02 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/22 09:23:17 by vda-conc         ###   ########.fr       */
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

int ft_parse_args(int ac, char **av)
{
  int i;

  i = 1;
  if (ac < 5 || ac > 6)
  {
    printf("Wrong number of arguments\n");
    return (0);
  }
  while (i < ac)
  {
    if (ft_atoi(av[i]) < 0)
    {
      printf("Wrong argument\n");
      return (0);
    }
    i++;
  }
  return (1);
}