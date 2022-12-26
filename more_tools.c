/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yomari <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 12:04:44 by yomari            #+#    #+#             */
/*   Updated: 2022/12/26 12:45:53 by yomari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

void	set_done(t_data *data)
{
	pthread_mutex_lock(data->main);
	data->done = 1;
	pthread_mutex_unlock(data->main);
}

void	set_time_nd_meal(t_data *data, size_t *time, int *meal, int i)
{
	pthread_mutex_lock(data->main);
	*time = get_time() - data->philos[i].last_meal;
	*meal = data->philos[i].meals;
	pthread_mutex_unlock(data->main);
}

void	print_dead(t_data *data, int i)
{
	pthread_mutex_lock(data->pr);
	printf("%ld philo n : %d is dead !\n", \
			get_time() - data->stime, data->philos[i].id + 1);
	pthread_mutex_unlock(data->pr);
}

int	check_meals(t_data *data, int count)
{
	if (count == data->n_ph)
	{
		set_done(data);
		return (1);
	}
	return (0);
}
