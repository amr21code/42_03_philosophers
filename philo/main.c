/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anruland <anruland@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 17:43:27 by anruland          #+#    #+#             */
/*   Updated: 2022/06/11 08:02:47 by anruland         ###   ########.fr       */
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

void	*ph_dinner(void *arg)
{
	t_philo			*philo;
	// char			*nb;
	int				size_nb;
	char			*now;

	philo = (t_philo *)arg;
	// nb = ft_itoa(philo->philo_no);
	gettimeofday(&philo->time, NULL);
	now = ft_itoa((philo->time.tv_usec / 1000) - philo->data->start);
	size_nb = ft_strlen(now);
	pthread_mutex_lock(&philo->data->talk);
	write(1, now, size_nb);
	write(1, " test\n", 6);
	pthread_mutex_unlock(&philo->data->talk);
	free(now);
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
