/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anruland <anruland@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 17:43:27 by anruland          #+#    #+#             */
/*   Updated: 2022/06/14 16:22:15 by anruland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ph_start_eating(t_philo *philo)
{
	int	fork1;
	int	fork2;
	int	time;

	fork1 = philo->fork_r;
	fork2 = *(philo->fork_l);
	if (philo->philo_no % 2 == 0)
	{
		fork1 = *(philo->fork_l);
		fork2 = philo->fork_r;
	}
	if (!pthread_mutex_lock(&philo->data->forks[fork1]))
	{
		ph_talk(philo, rfork);
		if (!pthread_mutex_lock(&philo->data->forks[fork2]))
		{
			ph_talk(philo, rfork);
			time = ph_talk(philo, reat);
			philo->last_eat = time;
			philo->no_eat++;
		}
	}
}

int	ph_check_meal_count(t_philo *philo)
{
	if (philo->data->no_times_eat >= 0
		&& philo->no_eat >= philo->data->no_times_eat)
	{
		pthread_mutex_unlock(&philo->data->forks[philo->fork_r]);
		pthread_mutex_unlock(&philo->data->forks[*philo->fork_l]);
		pthread_mutex_unlock(&philo->data->talk);
		return (1);
	}
	return (0);
}

void	*ph_dinner(void *arg)
{
	t_philo			*philo;
	int				time;

	philo = (t_philo *)arg;
	time = ph_get_current_time(philo->data->start);
	while (!philo->data->died)
	{
		ph_check_death(philo);
		if (ph_check_state(philo, time) == rthink)
			ph_talk(philo, rthink);
		if (ph_check_state(philo, time) == rsleep)
		{
			pthread_mutex_unlock(&philo->data->forks[philo->fork_r]);
			pthread_mutex_unlock(&philo->data->forks[*philo->fork_l]);
			ph_talk(philo, rsleep);
		}
		if (ph_check_state(philo, time) == reat)
			ph_start_eating(philo);
		time = ph_get_current_time(philo->data->start);
		if (ph_check_meal_count(philo))
			return (0);
		ph_check_death(philo);
	}
	if (philo->philo_no + 1 == philo->data->died)
		ph_talk(philo, rdied);
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
