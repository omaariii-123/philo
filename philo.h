#ifndef PHILO_H
#define PHILO_H
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/time.h>

typedef struct s_philo
{
	int	id;
	int	lfork;
	int	rfork;
	size_t	whentodie;
	int	dead_or_alive;
	pthread_t	philo;
	struct s_data	*data;
}t_philo;

typedef struct s_data
{
	size_t	ttd;
	size_t	tte;
	size_t	tts;
	size_t	stime;
	int	n_ph;
	t_philo *philos;
	pthread_mutex_t *forks;
	pthread_mutex_t *main;
	pthread_mutex_t *pr;

}t_data;


#endif
