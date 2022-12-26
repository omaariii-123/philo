/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yomari <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 03:37:38 by yomari            #+#    #+#             */
/*   Updated: 2022/12/26 11:15:07 by yomari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include"philo.h"

void	sleeping(t_philo *philo)
{
	print_action(philo->data, "is Sleeping ", philo->id);
	ft_usleep(philo->data->tts);
}

void	think(t_philo *philo)
{
	print_action(philo->data, "is Thinking", philo->id);
}

void	lock_for_eating(t_philo *philo, int n)
{
	pthread_mutex_lock(philo->data->main);
	if (n)
		philo->last_meal = get_time();
	if (!n)
	{
		if (philo->data->n_meals > 0 && philo->meals)
			philo->meals--;
	}
	philo->eating = n;
	pthread_mutex_unlock(philo->data->main);
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->data->main);
	if (philo->data->n_ph == 1)
	{
		pthread_mutex_unlock(philo->data->main);
		pthread_mutex_lock(philo->lfork);
		print_action(philo->data, "Has taken a fork", philo->id);
		philo->data->done = 1;
		pthread_mutex_unlock(philo->lfork);
		return ;
	}
	pthread_mutex_unlock(philo->data->main);
	pthread_mutex_lock(philo->lfork);
	pthread_mutex_lock(philo->rfork);
	print_fork(philo);
	lock_for_eating(philo, 1);
	print_action(philo->data, "is Eating", philo->id);
	ft_usleep(philo->data->tte);
	lock_for_eating(philo, 0);
	pthread_mutex_unlock(philo->rfork);
	pthread_mutex_unlock(philo->lfork);
}

void	update_done(t_philo *philo, bool *done)
{
	pthread_mutex_lock(philo->data->main);
	*done = philo->data->done;
	pthread_mutex_unlock(philo->data->main);
}
