/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anruland <anruland@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 17:43:27 by anruland          #+#    #+#             */
/*   Updated: 2022/06/12 19:51:18 by anruland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_putstr(char *str)
{
	while (*str)
	{
		write(1, str, 1);
		str++;
	}
}

char	*ph_message(int reason, int *state)
{
	if (reason == rfork)
	{
		*state = rfork;
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

void	*ph_dinner(void *arg)
{
	t_philo			*philo;
	struct timeval	time;
	int				timediff;
	char			*message;

	philo = (t_philo *)arg;
	gettimeofday(&time, NULL);
	timediff = (time.tv_usec / 1000) - philo->data->start;
	while ((philo->last_eat - timediff) < philo->data->time_die)
	{
		if (timediff > (philo->last_eat + philo->data->time_eat))
		{
			pthread_mutex_unlock(&philo->data->forks[philo->fork_r]);
			pthread_mutex_unlock(&philo->data->forks[*philo->fork_l]);
			message = ph_message(rsleep, &philo->state);
		}
		else if (timediff > (philo->last_eat + philo->data->time_eat + philo->data->time_sleep))
			message = ph_message(rthink, &philo->state);
		if (philo->state == rthink || (philo->state == rreadyeat))
		{
			if (!pthread_mutex_lock(&philo->data->forks[philo->fork_r])
				&& !pthread_mutex_lock(&philo->data->forks[*(philo->fork_l)]))
			{
				message = ph_message(reat, &philo->state);
				gettimeofday(&time, NULL);
				philo->last_eat = (time.tv_usec / 1000) - philo->data->start;
				philo->no_eat++;
			}
			else
				message = ph_message(rthink, &philo->state);
		}
		pthread_mutex_lock(&philo->data->talk);
		printf("%d %d %s\n", timediff, philo->philo_no + 1, message);
		pthread_mutex_unlock(&philo->data->talk);
		sleep(1);
		gettimeofday(&time, NULL);
		timediff = (time.tv_usec / 1000) - philo->data->start;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_table	data;
	t_philo	*philo;
	int		i;

	i = 0;
	if (!ph_error_check(&data, ac, av))
		return (-1);
	philo = ph_init_philos(&data);
	if (!philo)
		return (ft_printerror("Error: alloc of philo failed\n"));
	gettimeofday(&data.time, NULL);
	data.start = data.time.tv_usec / 1000;
	while (i < data.no_philo)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	ph_destructor(philo, &data);
}
