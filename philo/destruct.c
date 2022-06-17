/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destruct.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anruland <anruland@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 19:14:11 by anruland          #+#    #+#             */
/*   Updated: 2022/06/17 15:26:14 by anruland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * destroys a single pointer and protects it
 * @param arr [void *] pointer to destroy
 */
void	ph_destroy_single(void *arr)
{
	if (arr)
	{
		free(arr);
		arr = NULL;
	}
}

/**
 * destructor for destroying the mutexes and freeing allocations
 * @param philo [t_philo *] philosophers array
 * @param data [t_table *] data struct
 */
void	ph_destructor(t_philo *philo, t_table *data)
{
	int	i;

	i = 0;
	while (i < data->no_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		if (data->no_philo > 1)
			pthread_mutex_destroy(&philo[i].no_eat_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->talk);
	ph_destroy_single(data->forks);
	ph_destroy_single(data);
	ph_destroy_single(philo);
}
