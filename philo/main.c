/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anruland <anruland@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 17:43:27 by anruland          #+#    #+#             */
/*   Updated: 2022/06/11 10:22:38 by anruland         ###   ########.fr       */
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

char	*ph_message(int reason)
{
	if (reason == rfork)
		return ("has taken a fork");
	else if (reason == reat)
		return ("is eating");
	else if (reason == rsleep)
		return ("is sleeping");
	else if (reason == rthink)
		return ("is thinking");
	else
		return ("died");
}

void	*ph_dinner(void *arg)
{
	t_philo			*philo;
	struct timeval	time;
	int				timediff;
	char			*message;

	philo = (t_philo *)arg;
	message = ph_message(rsleep);
	gettimeofday(&time, NULL);
	timediff = (time.tv_usec / 1000) - philo->data->start;
	pthread_mutex_lock(&philo->data->talk);
	printf("%d %d %s\n", timediff, philo->philo_no, message);
	pthread_mutex_unlock(&philo->data->talk);
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
