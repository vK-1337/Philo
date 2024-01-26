/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vda-conc <vda-conc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 17:07:54 by vda-conc          #+#    #+#             */
/*   Updated: 2024/01/26 17:08:02 by vda-conc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}
int ft_is_space(const char c)
{
  return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ');
}

int ft_is_digit(const char c)
{
  return (c >= '0' && c <= '9');
}

long ft_atol(const char *str)
{
  long num;

  num = 0;
  str = ft_valid_input(str);
  while (ft_is_digit(*str))
  {
    num = (num * 10) + (*str - 48);
    ++str;
  }
  if (num > 2147483647)
    ft_error_exit("int max value is 2147483647");
  return (num);
}

const char *ft_valid_input(const char *str)
{
  int len;
  const char *number;

  len = 0;
  while (ft_is_space(*str))
    ++str;
  if (*str == '+')
    ++str;
  else if (*str == '-')
    ft_error_exit("Only positive value");
  if (!ft_is_digit(*str))
    ft_error_exit("Input is not a correct digit");
  number = str;
  while (ft_is_digit(*str))
  {
    ++str;
    ++len;
  }
  if (len > 10)
    ft_error_exit("Input is too long");
  return (number);
}