/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_helper2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anruland <anruland@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 12:12:34 by anruland          #+#    #+#             */
/*   Updated: 2022/06/17 18:44:05 by anruland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ph_rw_no_eat(t_philo *philo, int rw)
{
	pthread_mutex_lock(&philo->no_eat_mutex);
	if (rw == 0)
		return (philo->no_eat);
	else if (rw == 1)
		philo->no_eat++;
	pthread_mutex_unlock(&philo->no_eat_mutex);
	return (0);
}

int	ph_rw_last_eat(t_philo *philo, int rw, int time)
{
	pthread_mutex_lock(&philo->last_eat_mutex);
	if (rw == 0)
		return (philo->last_eat);
	else if (rw == 1)
		philo->last_eat = time;
	pthread_mutex_unlock(&philo->last_eat_mutex);
	return (0);
}

int	ph_one_philo(t_philo *philo, t_table *data)
{
	printf("2 1 is thinking\n");
	printf("4 1 has taken a fork\n");
	printf("%d 1 died\n", data->time_die + 2);
	ph_destructor(philo, data);
	return (0);
}
