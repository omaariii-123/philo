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
size_t get_time(void);

void	print_action(t_data *data, char *str, int id)
{
	bool done;

	pthread_mutex_lock(data->main);
	done = data->done;
	pthread_mutex_unlock(data->main);
	if(!done)
	{
		pthread_mutex_lock(data->pr);
		printf("%ld philo n : %d %s \n",get_time() - data->stime, id, str);	
		pthread_mutex_unlock(data->pr);
	}
}

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
	//wallet->philos[n].whentodie = get_time() - wallet->stime + wallet->ttd ;
	//wallet->philos[n].last_meal = get_time() - wallet->stime;
	wallet->philos[n].last_meal = wallet->stime;
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
	wallet->done = 0;
}
void	sleeping(t_philo *philo)
{
	print_action(philo->data, "is Sleeping ", philo->id);	
		ft_usleep(philo->data->tts);

}
void	think(t_philo *philo)
{

	print_action(philo->data, "is Thinking", philo->id);	
}

void	eat(t_philo *philo)
{

	/*if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->rfork);
		pthread_mutex_lock(philo->lfork);
	}
	else
	{*/
		pthread_mutex_lock(philo->lfork);
		pthread_mutex_lock(philo->rfork);
//	}
	print_action(philo->data, "Has taken a fork", philo->id);	
	print_action(philo->data, "Has taken a fork", philo->id);	

		pthread_mutex_lock(philo->data->main);
			philo->last_meal = get_time() ;
			philo->eating = 1;
		pthread_mutex_unlock(philo->data->main);
	print_action(philo->data, "is Eating", philo->id);	
	
	
		ft_usleep(philo->data->tte);
		
		pthread_mutex_lock(philo->data->main);
			philo->eating = 0;
		pthread_mutex_unlock(philo->data->main);
	
	
	
	/*if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->lfork);
		pthread_mutex_unlock(philo->rfork);
	}
	else
	{*/	
		pthread_mutex_unlock(philo->rfork);
		pthread_mutex_unlock(philo->lfork);
	//}

}
void	*routine(void *data)
{
	t_philo *philo;

	int i = 0;
	philo = (t_philo *)data;
	
	bool done;
	if (philo->id % 2 != 0)
		ft_usleep(100);
	while(1)
	{
		pthread_mutex_lock(philo->data->main);
		done = philo->data->done;
		if (done)
		{
			pthread_mutex_unlock(philo->data->main);
			break;
		}
		pthread_mutex_unlock(philo->data->main);
		if(!done)
		{
			eat(philo);
		}
		else
			break;

		pthread_mutex_lock(philo->data->main);
		done = philo->data->done;
		pthread_mutex_unlock(philo->data->main);
		
		if(!done)
			sleeping(philo);
		else
			break;
		
		pthread_mutex_lock(philo->data->main);
		done = philo->data->done;
		pthread_mutex_unlock(philo->data->main);
		if(!done)
			think(philo);
		else
			break;
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


	size_t	time;
	while(i < data->n_ph)
	{
		
			pthread_mutex_lock(data->main);
			time  = get_time() - data->philos[i].last_meal;
			//time  = get_time() - data->philos[i].last_meal;
			pthread_mutex_unlock(data->main);
		if ( time >= data->ttd && (data->philos[i].eating == 0) )
		{
			pthread_mutex_lock(data->main);
				pthread_mutex_lock(data->pr);
					printf("%ld philo n : %d is dead !\n", get_time() - data->stime , data->philos[i].id);
				pthread_mutex_unlock(data->pr);
			//data->philos[i].dead_or_alive = 0;
			data->done = 1;
			pthread_mutex_unlock(data->main);

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
	i = 0;
	wallet.stime = get_time();
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
	while(!flag)
	{
		checkk = check(&wallet);
		if(checkk == 1)
		{
			flag = 1;
		}
	}
	i = 0;
	while(i < wallet.n_ph)
	{
		pthread_join(wallet.philos[i].philo, NULL);	
		i++;
	}
	free_all(&wallet);
	return(0);
}
