/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anruland <anruland@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 19:10:12 by anruland          #+#    #+#             */
/*   Updated: 2022/06/10 19:12:38 by anruland         ###   ########.fr       */
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
	data->no_philo = ft_atoi(av[1]);
	data->no_forks = ft_atoi(av[1]);
	data->time_die = ft_atoi(av[2]);
	data->time_eat = ft_atoi(av[3]);
	data->time_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->no_times_eat = ft_atoi(av[5]);
	data->forks = (int *)malloc(sizeof(int) * data->no_forks);
	pthread_mutex_init(&data->eat, NULL);
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
	philo = (t_philo *)malloc(sizeof(t_philo) * data->no_philo);
	while (i < data->no_philo)
	{
		philo[i].data = data;
		philo[i].philo_no = i + 1;
		pthread_create(&philo[i].thread, NULL, ph_dinner, &philo[i]);
		i++;
	}
	return (philo);
}
