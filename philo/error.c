/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anruland <anruland@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 19:13:09 by anruland          #+#    #+#             */
/*   Updated: 2022/06/13 10:44:51 by anruland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * checks the input arguments for errors
 * - too little or too many arguments
 * - zero or negative number for the arguments
 * @param data [t_table *] unique data for everyone to use
 * @param av [char **] arguments of the program
 * @param ac [int] number of arguments of the program
 * @return 0 for error, 1 for success
 */
int	ph_error_check(t_table *data, int ac, char **av)
{
	int	philos;

	if (ac < 5 || ac > 6)
	{
		printf("Usage: %s no_philos time_die(ms) time_eat(ms) time_sleep(ms) \
			[no_times_eat]\n", av[0]);
		return (0);
	}
	philos = ft_atoi(av[1]);
	if (philos < 1)
	{
		printf("Error invalid arguments\n");
		return (0);
	}
	ph_data_init(data, av, ac);
	if (data->time_die < 1 || data->time_eat < 1 || data->time_sleep < 1)
	{
		printf("Error invalid arguments\n");
		return (0);
	}
	if (ac == 6 && data->no_times_eat < 1 && !data->debug)
	{
		printf("Error invalid arguments\n");
		return (0);
	}
	return (1);
}
