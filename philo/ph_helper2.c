/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_helper2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anruland <anruland@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 12:12:34 by anruland          #+#    #+#             */
/*   Updated: 2022/06/18 19:27:35 by anruland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ph_rw_no_eat(t_philo *philo, int rw)
{
	int	no_eat;

	no_eat = 0;
	pthread_mutex_lock(&philo->no_eat_mutex);
	if (rw == 0)
		no_eat = philo->no_eat;
	else if (rw == 1)
		philo->no_eat++;
	pthread_mutex_unlock(&philo->no_eat_mutex);
	return (no_eat);
}

int	ph_rw_last_eat(t_philo *philo, int rw, int time)
{
	int	last_eat;

	last_eat = 0;
	pthread_mutex_lock(&philo->last_eat_mutex);
	if (rw == 0)
		last_eat = philo->last_eat;
	else if (rw == 1)
		philo->last_eat = time;
	pthread_mutex_unlock(&philo->last_eat_mutex);
	return (last_eat);
}

int	ph_one_philo(t_philo *philo, t_table *data)
{
	printf("2 1 is thinking\n");
	printf("4 1 has taken a fork\n");
	printf("%d 1 died\n", data->time_die + 2);
	ph_destructor(philo, data);
	return (0);
}

int	ph_add_rem_fork(t_philo *philo, int fork)
{
	if (fork == philo->fork_r)
		return (2);
	else if (fork == *(philo->fork_l))
		return (1);
	return (0);
}

void	ph_unlock_mutexes(t_philo *philo)
{
	if (philo->forks >= 2)
	{
		pthread_mutex_unlock(&philo->data->forks[philo->fork_r]);
		philo->forks -= ph_add_rem_fork(philo, philo->fork_r);
	}
	if (philo->forks == 1)
		pthread_mutex_unlock(&philo->data->forks[*philo->fork_l]);
}
