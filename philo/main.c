/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anruland <anruland@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 17:43:27 by anruland          #+#    #+#             */
/*   Updated: 2022/06/13 12:54:08 by anruland         ###   ########.fr       */
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
		// *state = rfork;
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

void	ph_talk(t_philo *philo, int time, char *message)
{
	pthread_mutex_lock(&philo->data->talk);
	if (philo->data->debug)
		printf("last eat %d - ", philo->last_eat);
	printf("%d %d %s\n", time, philo->philo_no + 1, message);
	pthread_mutex_unlock(&philo->data->talk);
}

int	ph_get_current_time(long start)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000 - start);
}

int	ph_check_state(t_philo *philo, int time)
{
	if ((time > (philo->last_eat + philo->data->time_cycle)
			&& philo->state == rsleep)
		|| !philo->state
		|| philo->state == rreadyeat)
		return (rthink);
	else if (philo->state != rsleep
		&& time > (philo->last_eat + philo->data->time_eat)
		&& time < (philo->last_eat + philo->data->time_cycle))
		return (rsleep);
	else if (philo->state == rthink || (philo->state == rreadyeat))
		return (reat);
	return (0);
}

int	ph_check_death(t_philo *philo)
{
	int	time;

	time = ph_get_current_time(philo->data->start);
	if (time > philo->last_eat + philo->data->time_die)
	{
		philo->data->died = philo->philo_no + 1;
		return (1);
	}
	return (0);
}

void	*ph_dinner(void *arg)
{
	t_philo			*philo;
	int				time;
	char			*message;

	philo = (t_philo *)arg;
	time = ph_get_current_time(philo->data->start);
	while (!philo->data->died)
	{
		if (ph_check_state(philo, time) == rthink)
		{
			message = ph_message(rthink, &philo->state);
			time = ph_get_current_time(philo->data->start);
			ph_talk(philo, time, message);
		}
		if (ph_check_state(philo, time) == rsleep)
		{
			time = ph_get_current_time(philo->data->start);
			pthread_mutex_unlock(&philo->data->forks[philo->fork_r]);
			pthread_mutex_unlock(&philo->data->forks[*philo->fork_l]);
			message = ph_message(rsleep, &philo->state);
			ph_talk(philo, time, message);
		}
		if (ph_check_state(philo, time) == reat)
		{
			if (!pthread_mutex_lock(&philo->data->forks[philo->fork_r]))
			{
				if (ph_check_death(philo))
					break ;
				time = ph_get_current_time(philo->data->start);
				message = ph_message(rfork, &philo->state);
				ph_talk(philo, time, message);
				if (!pthread_mutex_lock(&philo->data->forks[*(philo->fork_l)]))
				{
					if (ph_check_death(philo))
						break ;
					time = ph_get_current_time(philo->data->start);
					message = ph_message(rfork, &philo->state);
					ph_talk(philo, time, message);
					message = ph_message(reat, &philo->state);
					time = ph_get_current_time(philo->data->start);
					ph_talk(philo, time, message);
					philo->last_eat = time;
					philo->no_eat++;
				}
			}
		}
		time = ph_get_current_time(philo->data->start);
		if (philo->data->no_times_eat >= 0 && philo->no_eat >= philo->data->no_times_eat)
		{
			pthread_mutex_unlock(&philo->data->forks[philo->fork_r]);
			pthread_mutex_unlock(&philo->data->forks[*philo->fork_l]);
			pthread_mutex_unlock(&philo->data->talk);
			return (0);
		}
	}
	if (philo->philo_no + 1 == philo->data->died)
	{
		time = ph_get_current_time(philo->data->start);
		message = ph_message(rdied, &philo->state);
		ph_talk(philo, time, message);
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
	data.start = data.time.tv_sec * 1000 + data.time.tv_usec / 1000;
	while (i < data.no_philo)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	ph_destructor(philo, &data);
}
