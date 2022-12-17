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
	wallet->philos[n].whentodie = wallet->stime + wallet->ttd;
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

void	think(t_philo *philo)
{
//	pthread_mutex_lock(philo->data->main);
	pthread_mutex_lock(philo->data->pr);
	printf("%ld philo n : %d is sleeping\n",get_time() - philo->data->stime, philo->id);
	pthread_mutex_unlock(philo->data->pr);
	ft_usleep(philo->data->tts);
	pthread_mutex_lock(philo->data->pr);
 	printf("%ld philo n : %d is thinking\n",get_time() - philo->data->stime, philo->id);	
	pthread_mutex_unlock(philo->data->pr);
//	pthread_mutex_unlock(philo->data->main);
}

void	eat(t_philo *philo)
{
	//if (philo->id % 2 == 0)
	//	usleep(20);
	pthread_mutex_lock(&philo->data->forks[philo->lfork]);
	pthread_mutex_lock(&philo->data->forks[philo->rfork]);	
	pthread_mutex_lock(philo->data->pr);
 	printf("%ld philo n : %d is eating\n",get_time() - philo->data->stime, philo->id);	
 	pthread_mutex_unlock(philo->data->pr);
	ft_usleep(philo->data->tte);

	pthread_mutex_lock(philo->data->main);
	philo->whentodie += philo->data->ttd;
	pthread_mutex_unlock(philo->data->main);

	pthread_mutex_unlock(&philo->data->forks[philo->rfork]);
	pthread_mutex_unlock(&philo->data->forks[philo->lfork]);
}
void	*routine(void *data)
{
	int	i;
	t_philo *philo;

	philo = (t_philo *)data;
	while(1)
	{
		pthread_mutex_lock(philo->data->main);
		if(philo->dead_or_alive == 0)
		{
			pthread_mutex_unlock(philo->data->main);
			break;
		}
		pthread_mutex_unlock(philo->data->main);
		eat(philo);
		think(philo);
	}
	return(NULL);
}
int	main(int ac, char **av)
{
	t_data wallet;
	int	i = 0;
	int	d;
	void	*ptr;

	
	fill_data(&wallet, av);
	init_mutex(&wallet);
	wallet.stime = get_time();
	while (i < wallet.n_ph)
	{
		init_philo(&wallet, i);
		pthread_create(&wallet.philos[i].philo, NULL, &routine, &wallet.philos[i]);
		usleep(15);
		i++;
	}
	i = 0;
	while (i < wallet.n_ph)
	{
		pthread_join(wallet.philos[i].philo, NULL);
		i++;
	}

}
