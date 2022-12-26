/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yomari <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 01:34:20 by yomari            #+#    #+#             */
/*   Updated: 2022/12/26 12:46:17 by yomari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include<stdio.h>
# include<stdlib.h>
# include<pthread.h>
# include<unistd.h>
# include<sys/time.h>
# include <stdbool.h>

typedef struct s_philo
{
	int				id;
	int				meals;
	pthread_mutex_t	*lfork;
	pthread_mutex_t	*rfork;
	int				eating;
	size_t			last_meal;
	pthread_t		philo;
	struct s_data	*data;
}t_philo;

typedef struct s_data
{
	pthread_mutex_t	*forks;
	pthread_mutex_t	*main;
	pthread_mutex_t	*pr;
	size_t			ttd;
	size_t			tte;
	size_t			tts;
	size_t			stime;
	int				n_ph;
	int				n_meals;
	bool			done;
	t_philo			*philos;
}t_data;

size_t	get_time(void);
void	ft_usleep(size_t n);
void	fill_data(t_data *wallet, char **av, int ac);
void	init_mutex(t_data *wallet);
void	init_philo(t_data *wallet, int n);
void	*routine(void *data);
void	print_action(t_data *data, char *str, int id);
void	print_fork(t_philo *philo);
void	eat(t_philo *philo);
void	sleeping(t_philo *philo);
void	think(t_philo *philo);
void	lock_for_eating(t_philo *philo, int n);
void	update_done(t_philo *philo, bool *done);
void	destroy(t_data *data);
int		check(t_data *data);
void	free_all(t_data *data);
int		check_errors(int ac, char **av);
int		ft_atoi(char *str);
void	set_done(t_data *data);
void	set_time_nd_meal(t_data *data, size_t*time, int *meal, int i);
void	print_dead(t_data *data, int i);
int		check_meals(t_data *data, int count);

#endif
