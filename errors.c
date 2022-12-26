/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yomari <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 05:02:53 by yomari            #+#    #+#             */
/*   Updated: 2022/12/26 12:22:18 by yomari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

int	ft_atoi(char *str)
{
	int		neg;
	long	num;
	int		i;

	i = 0;
	num = 0;
	while (str[i] <= ' ')
		i++;
	neg = 1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			neg *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
		num = num * 10 + (str[i++] - 48);
	return (num * neg);
}

int	ft_is_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	if (i == 0)
		return (0);
	return (1);
}

int	check_errors(int ac, char **av)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (!ft_is_digit(av[i]) || !ft_atoi(av[i]))
			return (1);
		i++;
	}
	if (ac > 6)
		return (1);
	else if (ac < 5)
		return (1);
	return (0);
}
