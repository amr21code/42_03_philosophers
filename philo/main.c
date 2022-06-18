/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anruland <anruland@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 17:43:27 by anruland          #+#    #+#             */
/*   Updated: 2022/06/18 19:27:57 by anruland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ph_start_eating(t_philo *philo)
{
	int	fork1;
	int	fork2;
	int	time;

	time = 0;
	fork1 = philo->fork_r;
	fork2 = *(philo->fork_l);
	if (philo->philo_no % 2 == 0 || ph_rw_no_eat(philo, 0) > 0)
	{
		fork1 = *(philo->fork_l);
		fork2 = philo->fork_r;
	}
	if (!pthread_mutex_lock(&philo->data->forks[fork1]))
	{
		time = ph_talk(philo, rfork);
		philo->forks += ph_add_rem_fork(philo, fork1);
		if (!pthread_mutex_lock(&philo->data->forks[fork2]))
		{
			ph_talk(philo, rfork);
			time = ph_talk(philo, reat);
			ph_rw_last_eat(philo, 1, time);
			ph_rw_no_eat(philo, 1);
			philo->forks += ph_add_rem_fork(philo, fork2);
		}
	}
}

int	ph_check_meal_count(t_philo *philo)
{
	if (philo->data->no_times_eat >= 0
		&& philo->no_eat > philo->data->no_times_eat)
		return (1);
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
	while (!data->died && (data->no_times_eat == -1 || (data->no_times_eat != -1
				&& times_ate < data->no_times_eat)))
	{
		i = 0;
		while (i < data->no_philo && !data->died)
		{
			if (ph_check_death(&philo[i]))
				ph_talk(&philo[i], rdied);
			if (data->no_times_eat > 0
				&& ph_rw_no_eat(philo, 0) >= data->no_times_eat)
				times_ate++;
			i++;
		}
	}
	return (0);
}

void	*ph_dinner(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (philo->state != rdied)
	{
		if (philo->state != rdied && ph_check_state(philo) == rthink)
			ph_talk(philo, rthink);
		if (philo->state != rdied && ph_check_state(philo) == rsleep)
		{
			ph_talk(philo, rsleep);
			pthread_mutex_unlock(&philo->data->forks[philo->fork_r]);
			pthread_mutex_unlock(&philo->data->forks[*philo->fork_l]);
			philo->forks -= 3;
		}
		if (philo->state != rdied && ph_check_state(philo) == reat)
			ph_start_eating(philo);
		if (philo->state != rdied && ph_check_meal_count(philo))
		{
			philo->state = rdied;
			break ;
		}
	}
	ph_unlock_mutexes(philo);
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
		return (ft_printerror(ph_destructor(philo, data)));
	if (data->no_philo == 1)
		return (ph_one_philo(philo, data));
	ph_init_death(data, philo);
	pthread_join(data->death, NULL);
	while (i < data->no_philo)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	ph_destructor(philo, data);
}
