/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anruland <anruland@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 17:43:27 by anruland          #+#    #+#             */
/*   Updated: 2022/06/17 14:38:06 by anruland         ###   ########.fr       */
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
	// printf("philo %d - took f1 %d and f2 %d\n", philo->philo_no + 1, fork1, fork2);
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

void	*ph_death(void *arg)
{
	t_philo	*philo;
	t_table	*data;
	int		i;
	int		times_ate;

	times_ate = 0;
	philo = (t_philo *)arg;
	data = philo[0].data;
	while (!data->died && times_ate < data->no_times_eat)
	{
		i = 0;
		while (i < data->no_philo && !data->died)
		{
			if (ph_check_death(&philo[i]))
				ph_talk(&philo[i], rdied);
			if (data->no_times_eat > 0 && philo[i].no_eat >= data->no_times_eat)
				times_ate++;
			i++;
		}
	}
	return (0);
}

void	*ph_dinner(void *arg)
{
	t_philo			*philo;

	philo = (t_philo *)arg;
	while (!philo->data->died)
	{
		if (ph_check_state(philo) == rthink)
			ph_talk(philo, rthink);
		if (ph_check_state(philo) == rsleep)
		{
			ph_talk(philo, rsleep);
			pthread_mutex_unlock(&philo->data->forks[philo->fork_r]);
			pthread_mutex_unlock(&philo->data->forks[*philo->fork_l]);
		}
		if (ph_check_state(philo) == reat)
			ph_start_eating(philo);
		if (ph_check_meal_count(philo))
			return (0);
		// ph_check_death(philo);
	}
	pthread_mutex_unlock(&philo->data->forks[philo->fork_r]);
	pthread_mutex_unlock(&philo->data->forks[*philo->fork_l]);
	// if (philo->philo_no + 1 == philo->data->died)
	// 	ph_talk(philo, rdied);
	return (0);
}

int	main(int ac, char **av)
{
	t_table	*data;
	t_philo	*philo;
	int		i;

	i = 0;
	data = (t_table *)malloc(sizeof(t_table));
	if (!ph_error_check(data, ac, av))
		return (-1);
	gettimeofday(&data->time, NULL);
	data->start = data->time.tv_sec * 1000 + data->time.tv_usec / 1000;
	philo = ph_init_philos(data);
	if (data->no_philo > 1 && !philo)
	{
		ph_destructor(philo, data);
		return (ft_printerror("Error: alloc of philo failed\n"));
	}
	if (data->no_philo == 1)
	{
		printf("2 1 is thinking\n");
		printf("4 1 has taken a fork\n");
		printf("%d 1 died\n", data->time_die + 2);
		ph_destructor(philo, data);
		return (0);
	}
	ph_init_death(data, philo);
	pthread_join(data->death, NULL);
	while (i < data->no_philo)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	ph_destructor(philo, data);
}
