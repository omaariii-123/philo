/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yomari <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 23:58:58 by yomari            #+#    #+#             */
/*   Updated: 2022/12/19 01:31:03 by yomari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

size_t get_time(void)
{
	static struct timeval t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void	ft_usleep(size_t n)
{
	size_t end = n + get_time();
	while (get_time() < end)
	{
		usleep(50);	
	}
}


void	init_philo(t_data *wallet,int n)
{
	wallet->philos[n].id = n;
	wallet->philos[n].lfork = n;
	wallet->philos[n].rfork = (n + 1) % wallet->n_ph;
	wallet->philos[n].data = wallet;
	wallet->philos[n].whentodie = get_time() - wallet->stime + wallet->ttd ;
	wallet->philos[n].last_meal = 0;
	wallet->philos[n].eating = 0;
}
void	init_mutex(t_data *wallet)
{
	int	i = 0;

	while(i < wallet->n_ph)
	{
		pthread_mutex_init(&wallet->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(wallet->main, NULL);
	pthread_mutex_init(wallet->pr, NULL);
}

void	fill_data(t_data *wallet, char **av)
{
	int	n;

	n = atoi(av[1]);
	wallet->ttd = atoi(av[2]);
	wallet->tte = atoi(av[3]);
	wallet->tts = atoi(av[4]);
	wallet->philos = malloc(sizeof(t_philo) * n);
	wallet->forks = malloc(sizeof(pthread_mutex_t) * n);
	wallet->main = malloc(sizeof(pthread_mutex_t));
	wallet->pr = malloc(sizeof(pthread_mutex_t));
	wallet->n_ph = n;
}
void	sleeping(t_philo *philo)
{
	pthread_mutex_lock(philo->data->pr);
	printf("%ld philo n : %d is sleeping\n",get_time() - philo->data->stime, philo->id);
	pthread_mutex_unlock(philo->data->pr);
	ft_usleep(philo->data->tts);

}
void	think(t_philo *philo)
{
	
	pthread_mutex_lock(philo->data->pr);
 	printf("%ld philo n : %d is thinking\n",get_time() - philo->data->stime, philo->id);	
	pthread_mutex_unlock(philo->data->pr);

}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->data->main);
	philo->last_meal = get_time() - philo->data->stime;
	philo->eating = 1;
	pthread_mutex_unlock(philo->data->main);

	pthread_mutex_lock(&philo->data->forks[philo->lfork]);
	pthread_mutex_lock(&philo->data->forks[philo->rfork]);
	
	pthread_mutex_lock(philo->data->pr);
 	printf("%ld philo n : %d is eating\n",get_time() - philo->data->stime, philo->id);	
 	pthread_mutex_unlock(philo->data->pr);
	ft_usleep(philo->data->tte);
	pthread_mutex_unlock(&philo->data->forks[philo->rfork]);
	pthread_mutex_unlock(&philo->data->forks[philo->lfork]);

	pthread_mutex_lock(philo->data->main);
	philo->eating = 0;
	pthread_mutex_unlock(philo->data->main);


}
void	*routine(void *data)
{
	t_philo *philo;

	philo = (t_philo *)data;
	while(1)
	{

		eat(philo);
		sleeping(philo);
		think(philo);
	}
}
void	destroy(t_data *data)
{
	int	i = 0;

	while (i < data->n_ph)
	{
		pthread_mutex_destroy(&data->forks[i]);
		free(&data->philos[i]);
		i++;
	}
	//pthread_mutex_unlock(data->pr);
	//pthread_mutex_unlock(data->main);

	pthread_mutex_destroy(data->pr);
	pthread_mutex_destroy(data->main);


}
void call(t_data *data)
{

	ft_usleep(data->ttd);
	//pthread_mutex_unlock(data->pr);
}
int check(t_data *data)
{
	int	i = 0;

	while(i < data->n_ph)
	{
		pthread_mutex_lock(data->pr);
		pthread_mutex_lock(data->main);
		if ( (get_time() - data->philos[i].last_meal - data->stime) >= data->ttd && (data->philos[i].eating == 0))
		{
			printf("%ld philo n : %d is dead !\n", get_time() - data->stime , data->philos[i].id);
			call(data);
			return(1);
		}
		i++;
		pthread_mutex_unlock(data->main);
		pthread_mutex_unlock(data->pr);
	}
	return(0);
}
void	free_all(t_data *data)
{
	int	i = 0;

	free(data->philos);
	free(data->main);
	free(data->forks);
	free(data->pr);
}
int	main(int ac, char **av)
{
	t_data wallet;
	int	i;
	(void)ac;

	
	fill_data(&wallet, av);
	init_mutex(&wallet);
	wallet.stime = get_time();
	i = 0;
	while (i < wallet.n_ph)
	{
		init_philo(&wallet, i);
		if ((pthread_create(&wallet.philos[i].philo, NULL, &routine, &wallet.philos[i])!= 0))
				{		
					printf("error creating thread !\n");
					return 1;
					}
		i++;
	}
	i = 0;
	while (i < wallet.n_ph)
	{
		pthread_detach(wallet.philos[i].philo);
		i++;
	}

	while(1)
	{
		if (check(&wallet))
		{

		//	destroy(&wallet);
//			free_all(&wallet);
			break;
		}
	}

}
