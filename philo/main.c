/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anruland <anruland@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 17:43:27 by anruland          #+#    #+#             */
/*   Updated: 2022/06/09 09:40:23 by anruland         ###   ########.fr       */
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

void	*test(void *arg)
{
	write(1, "test\n", 5);
	return (arg);
}

int	main(int ac, char **av)
{
	pthread_t	philo;

	if (ac < 5 || ac > 7)
	{
		ft_putstr("Usage: ");
		ft_putstr(av[0]);
		ft_putstr(" no_philos time_die(ms) time_eat(ms) time_sleep(ms");
		ft_putstr(" [no_times_eat]\n");
	}
	pthread_create(&philo, NULL, test, NULL);
	pthread_join(philo, NULL);
}
