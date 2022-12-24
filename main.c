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
	size_t time = get_time();
	while (time< end)
	{
		usleep(50);
		time = get_time();
	}
}


void	init_philo(t_data *wallet,int n)
{
	wallet->philos[n].id = n;
	wallet->philos[n].lfork = &wallet->forks[n];
	wallet->philos[n].rfork =&wallet->forks[(n + 1) % wallet->n_ph];
	wallet->philos[n].data = wallet;
	wallet->philos[n].whentodie = get_time() - wallet->stime + wallet->ttd ;
	wallet->philos[n].last_meal = get_time() - wallet->stime;
	wallet->philos[n].eating = 0;
	wallet->philos[n].dead_or_alive = 1;
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
	pthread_mutex_lock(philo->data->main);
	if (!philo->done)
	{
		pthread_mutex_unlock(philo->data->main);
		pthread_mutex_lock(philo->data->pr);
		printf("%ld philo n : %d is sleeping\n",get_time() - philo->data->stime, philo->id);
		pthread_mutex_unlock(philo->data->pr);
	}
	else
		pthread_mutex_unlock(philo->data->main);
		ft_usleep(philo->data->tts);

}
void	think(t_philo *philo)
{
	pthread_mutex_lock(philo->data->main);
		if (philo->done != 1)
		{
			pthread_mutex_unlock(philo->data->main);
			pthread_mutex_lock(philo->data->pr);
		 	printf("%ld philo n : %d is thinking\n",get_time() - philo->data->stime, philo->id);	
			pthread_mutex_unlock(philo->data->pr);
		}
		else
			pthread_mutex_unlock(philo->data->main);

}

void	eat(t_philo *philo)
{
//	if(philo->dead_or_alive )
//	{
	if (philo->id % 2 == 0)
	{
	pthread_mutex_lock(philo->rfork);
	pthread_mutex_lock(philo->lfork);
	}
	else
	{
	pthread_mutex_lock(philo->lfork);
	pthread_mutex_lock(philo->rfork);
	}
	pthread_mutex_lock(philo->data->main);
	philo->last_meal = get_time() - philo->data->stime;
	philo->eating = 1;

	//if(philo->done || !philo->dead_or_alive)
		//return;
		if (philo->done != 1)
		{
			pthread_mutex_unlock(philo->data->main);
			pthread_mutex_lock(philo->data->pr);
		 	printf("%ld philo n : %d is eating\n",get_time() - philo->data->stime, philo->id);	
			pthread_mutex_unlock(philo->data->pr);
		}
		else
			pthread_mutex_unlock(philo->data->main);
	ft_usleep(philo->data->tte);
		
	pthread_mutex_lock(philo->data->main);
	philo->eating = 0;
	pthread_mutex_unlock(philo->data->main);
	
	if (philo->id % 2 == 0)
	{
	pthread_mutex_unlock(philo->lfork);
	pthread_mutex_unlock(philo->rfork);
	}
	else
	{
	
	pthread_mutex_unlock(philo->rfork);
	pthread_mutex_unlock(philo->lfork);
	}
//	}
//	else
//		return;


}
void	*routine(void *data)
{
	t_philo *philo;

	int i = 0;
	philo = (t_philo *)data;
	pthread_mutex_lock(philo->data->main);
	while(!philo->done)
	{
		if (i == 0)
		{	
			pthread_mutex_unlock(philo->data->main);
			i = 1337;
		}
//		if(philo->done || !philo->dead_or_alive)
//			return(NULL);
		eat(philo);
//		if(philo->done || !philo->dead_or_alive)
//			return(NULL);
		sleeping(philo);
//		if(philo->done || !philo->dead_or_alive)
//			return(NULL);
		think(philo);
//		if(philo->done || !philo->dead_or_alive)
//			return(NULL);
	}
	return (NULL);
}
void	destroy(t_data *data)
{
	int	i = 0;

	while (i < data->n_ph)
	{
		pthread_mutex_unlock(&data->forks[i]);
		pthread_mutex_destroy(&data->forks[i]);
		free(&data->philos[i]);
		i++;
	}
	pthread_mutex_unlock(data->pr);
	pthread_mutex_unlock(data->main);

	pthread_mutex_destroy(data->pr);
	pthread_mutex_destroy(data->main);


}
void call(t_data *data)
{

	ft_usleep(200);
}
void	free_all(t_data *data);
int check(t_data *data)
{
	int	i = 0;
	int	n = 0;

	size_t	time = get_time() - data->philos[i].last_meal - data->stime;
	
	while(i < data->n_ph)
	{
		
			printf("salam\n");
		if ( time >= data->ttd && (data->philos[i].eating == 0) )
		{
		//	pthread_mutex_unlock(data->main);
			pthread_mutex_lock(data->pr);
			printf("%ld philo n : %d is dead !\n", get_time() - data->stime , data->philos[i].id);
			pthread_mutex_unlock(data->pr);
			data->philos[i].dead_or_alive = 0;
			while(n < data->n_ph)
			{
				data->philos[n].done = 1;
				n++;
			}
		//	call(data);
			return(1);
		}
		i++;
//	pthread_mutex_unlock(data->main);
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
	int	checkk;
	int	flag = 0;
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
		usleep(50);
		i++;
	}
	while(!flag)
	{
		checkk = check(&wallet);
		if(checkk == 1)
		{
			flag = 1;
			//destroy(&wallet);
		}
	}
	i = 0;
	while(i < wallet.n_ph)
	{
//		if(wallet.n_ph == 1)
//		{
//			pthread_mutex_unlock(wallet.philos[0].lfork);
//			pthread_mutex_unlock(wallet.philos[0].rfork);
//		}
		pthread_join(wallet.philos[i].philo, NULL);	
		i++;
	}
	//free_all(&wallet);
	return(0);
}
