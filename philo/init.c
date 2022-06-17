/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anruland <anruland@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 19:10:12 by anruland          #+#    #+#             */
/*   Updated: 2022/06/17 13:32:11 by anruland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * initializes the data struct with the given arguments
 * also initializes the mutexes
 * @param data [t_table *] unique data for everyone to use
 * @param av [char **] arguments of the program
 * @param ac [int] number of arguments of the program
 * @return void
 */
void	ph_data_init(t_table *data, char **av, int ac)
{
	int	i;

	i = 0;
	data->no_philo = ft_atoi(av[1]);
	data->no_forks = ft_atoi(av[1]);
	data->time_die = ft_atoi(av[2]);
	data->time_eat = ft_atoi(av[3]);
	data->time_sleep = ft_atoi(av[4]);
	data->time_cycle = data->time_eat + data->time_sleep;
	data->died = 0;
	data->debug = 0;
	if (ac == 6 && av[5][0] != '-')
		data->no_times_eat = ft_atoi(av[5]);
	else
		data->no_times_eat = -1;
	if (ac == 6 && av[5][0] == '-')
		data->debug = 1;
	data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data->no_forks);
	while (i < data->no_forks)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&data->talk, NULL);
}

/**
 * initializes the philosophers by allocating memory for each
 * saves the unique data struct pointer to each and
 * spawns a thread
 * @param data [t_table *] unique data for everyone to use
 * @return the array of philosophers
 */
t_philo	*ph_init_philos(t_table *data)
{
	int		i;
	t_philo	*philo;

	i = 0;
	if (data->no_philo > 1)
		philo = (t_philo *)malloc(sizeof(t_philo) * data->no_philo);
	else
		philo = NULL;
	while (data->no_philo > 1 && i < data->no_philo)
	{
		philo[i].data = data;
		philo[i].philo_no = i;
		philo[i].forks = 0;
		philo[i].fork_r = i;
		philo[i].no_eat = 0;
		philo[i].state = 0;
		philo[i].last_eat = 0;
		if (i > 0 && i < data->no_philo - 1)
			philo[i].fork_l = &philo[i - 1].fork_r;
		else if (i != 0)
		{
			philo[0].fork_l = &philo[i].fork_r;
			philo[i].fork_l = &philo[i - 1].fork_r;
		}
		if (data->no_philo == 1)
			philo[0].fork_l = &philo[0].fork_r;
		pthread_create(&philo[i].thread, NULL, ph_dinner, &philo[i]);
		i++;
	}
	return (philo);
}

void	ph_init_death(t_table *data, t_philo *philo)
{
	pthread_create(&data->death, NULL, ph_death, philo);
}
