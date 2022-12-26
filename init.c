/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yomari <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 03:51:18 by yomari            #+#    #+#             */
/*   Updated: 2022/12/26 11:32:20 by yomari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include"philo.h"

size_t	get_time(void)
{
	static struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void	ft_usleep(size_t n)
{
	size_t	end;
	size_t	time;

	end = n + get_time();
	time = get_time();
	while (time < end)
	{
		usleep(50);
		time = get_time();
	}
}

void	init_philo(t_data *wallet, int n)
{
	wallet->philos[n].id = n;
	wallet->philos[n].lfork = &wallet->forks[n];
	wallet->philos[n].rfork = &wallet->forks[(n + 1) % wallet->n_ph];
	wallet->philos[n].data = wallet;
	wallet->philos[n].last_meal = wallet->stime;
	wallet->philos[n].eating = 0;
	wallet->philos[n].meals = wallet->n_meals;
}

void	init_mutex(t_data *wallet)
{
	int	i;

	i = 0;
	while (i < wallet->n_ph)
	{
		pthread_mutex_init(&wallet->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(wallet->main, NULL);
	pthread_mutex_init(wallet->pr, NULL);
}

void	fill_data(t_data *wallet, char **av, int ac)
{
	int	n;

	n = ft_atoi(av[1]);
	wallet->ttd = ft_atoi(av[2]);
	wallet->tte = ft_atoi(av[3]);
	wallet->tts = ft_atoi(av[4]);
	wallet->philos = malloc(sizeof(t_philo) * n);
	wallet->forks = malloc(sizeof(pthread_mutex_t) * n);
	wallet->main = malloc(sizeof(pthread_mutex_t));
	wallet->pr = malloc(sizeof(pthread_mutex_t));
	wallet->n_ph = n;
	wallet->done = 0;
	if (ac == 6)
		wallet->n_meals = ft_atoi(av[5]);
	if (ac == 5)
		wallet->n_meals = -1;
}
