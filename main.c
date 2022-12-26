/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yomari <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 23:58:58 by yomari            #+#    #+#             */
/*   Updated: 2022/12/26 12:58:42 by yomari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

void	thread_creation(t_data *wallet)
{
	int	i;

	i = 0;
	while (i < wallet->n_ph)
	{
		init_philo(wallet, i);
		pthread_create(&wallet->philos[i].philo, NULL, \
				&routine, &wallet->philos[i]);
		usleep(100);
		i++;
	}
}

void	*routine(void *data)
{
	t_philo	*philo;
	bool	done;

	philo = (t_philo *)data;
	while (1)
	{
		update_done(philo, &done);
		if (!done)
			eat(philo);
		else
			break ;
		update_done(philo, &done);
		if (!done)
			sleeping(philo);
		else
			break ;
		update_done(philo, &done);
		if (!done)
			think(philo);
		else
			break ;
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_data	wallet;
	int		i;
	int		flag ;

	if (check_errors(ac, av))
		return (1);
	fill_data(&wallet, av, ac);
	init_mutex(&wallet);
	wallet.stime = get_time();
	thread_creation(&wallet);
	flag = 0;
	while (!flag)
	{
		i = check(&wallet);
		if (i)
			flag = 1;
	}
	i = -1;
	while (++i < wallet.n_ph)
		pthread_join(wallet.philos[i].philo, NULL);
	destroy(&wallet);
	free_all(&wallet);
	return (0);
}
