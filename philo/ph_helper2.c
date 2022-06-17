/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_helper2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anruland <anruland@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 12:12:34 by anruland          #+#    #+#             */
/*   Updated: 2022/06/17 15:20:06 by anruland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ph_rw_no_eat(t_philo *philo, int rw)
{
	pthread_mutex_lock(&philo->no_eat_mutex);
	if (rw == 0)
		return (philo->no_eat);
	else if (rw == 1)
		philo->no_eat++;
	pthread_mutex_unlock(&philo->no_eat_mutex);
	return (0);
}
