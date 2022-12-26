/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yomari <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 03:56:46 by yomari            #+#    #+#             */
/*   Updated: 2022/12/26 12:47:15 by yomari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

void	print_action(t_data *data, char *str, int id)
{
	bool	done;

	pthread_mutex_lock(data->main);
	done = data->done;
	pthread_mutex_unlock(data->main);
	if (!done)
	{
		pthread_mutex_lock(data->pr);
		printf("%ld philo n : %d %s \n", get_time() - data->stime, id + 1, str);
		pthread_mutex_unlock(data->pr);
	}
}

void	print_fork(t_philo *philo)
{
	bool	done;

	pthread_mutex_lock(philo->data->main);
	done = philo->data->done;
	pthread_mutex_unlock(philo->data->main);
	if (!done)
	{
		pthread_mutex_lock(philo->data->pr);
		printf("%ld philo n : %d Has taken a fork  \n", \
				get_time() - philo->data->stime, philo->id + 1);
		printf("%ld philo n : %d Has taken a fork  \n", \
				get_time() - philo->data->stime, philo->id + 1);
		pthread_mutex_unlock(philo->data->pr);
	}
}

void	destroy(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_ph)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(data->pr);
	pthread_mutex_destroy(data->main);
}

int	check(t_data *data)
{
	int		i;
	size_t	time;
	int		meal;
	int		count;

	i = -1;
	count = 0;
	while (++i < data->n_ph)
	{
		set_time_nd_meal(data, &time, &meal, i);
		if (time >= data->ttd && (data->philos[i].eating == 0))
		{
			set_done(data);
			if (meal)
			{
				print_dead(data, i);
				return (1);
			}
		}
		if (!meal)
			count++;
	}
	if (check_meals(data, count))
		return (1);
	return (0);
}

void	free_all(t_data *data)
{
	free(data->philos);
	free(data->main);
	free(data->forks);
	free(data->pr);
}
