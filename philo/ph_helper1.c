/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_helper1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anruland <anruland@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 12:09:28 by anruland          #+#    #+#             */
/*   Updated: 2022/06/18 16:49:25 by anruland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

char	*ph_message(int reason, int *state)
{
	if (reason == rfork)
	{
		return ("has taken a fork");
	}
	else if (reason == reat)
	{
		*state = reat;
		return ("is eating");
	}
	else if (reason == rsleep)
	{
		*state = rsleep;
		return ("is sleeping");
	}
	else if (reason == rthink)
	{
		*state = rthink;
		return ("is thinking");
	}
	else
	{
		*state = rdied;
		return ("died");
	}
}

int	ph_talk(t_philo *philo, int reason)
{
	int		time;
	char	*message;
	int		died;

	pthread_mutex_lock(&philo->data->died_mutex);
	died = philo->data->died;
	pthread_mutex_unlock(&philo->data->died_mutex);
	time = ph_get_current_time(philo->data->start);
	if (reason != rdied)
		message = ph_message(reason, &philo->state);
	else
		message = "died";
	if (!died || reason == rdied)
	{
		pthread_mutex_lock(&philo->data->talk);
		printf("%d %d %s\n", time, philo->philo_no + 1, message);
		if (philo->data->debug)
		{
			printf("state %d\n", philo->state);
			printf("last eat %d\n", philo->last_eat);
			printf("died %d\n", philo->data->died);
			printf("-----\n");
		}
		pthread_mutex_unlock(&philo->data->talk);
	}
	return (time);
}

int	ph_get_current_time(long start)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000 - start);
}

int	ph_check_state(t_philo *philo)
{
	int	time;

	time = ph_get_current_time(philo->data->start);
	if ((time > (philo->last_eat + philo->data->time_cycle)
			&& philo->state == rsleep)
		|| !philo->state)
		return (rthink);
	else if (philo->state != rsleep
		&& time > (philo->last_eat + philo->data->time_eat)
		&& time < (philo->last_eat + philo->data->time_cycle))
		return (rsleep);
	else if (philo->state == rthink)
		return (reat);
	return (0);
}

int	ph_check_death(t_philo *philo)
{
	int	time;

	time = ph_get_current_time(philo->data->start);
	if (time > ph_rw_last_eat(philo, 0, 0) + philo->data->time_die)
	{
		pthread_mutex_lock(&philo->data->died_mutex);
		philo->data->died = philo->philo_no + 1;
		pthread_mutex_unlock(&philo->data->died_mutex);
		return (1);
	}
	return (0);
}
