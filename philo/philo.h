/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anruland <anruland@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 17:41:29 by anruland          #+#    #+#             */
/*   Updated: 2022/06/17 15:21:02 by anruland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

enum e_reasons
{
	rfork = 1,
	reat,
	rsleep,
	rthink,
	rdied,
};

typedef struct s_table
{
	int				no_forks;
	int				no_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				no_times_eat;
	int				time_cycle;
	long			start;
	int				died;
	struct timeval	time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	talk;
	pthread_t		death;
	int				debug;
}	t_table;

typedef struct s_philo
{
	int				philo_no;
	int				state;
	int				forks;
	int				*fork_l;
	int				fork_r;
	int				last_eat;
	int				no_eat;
	pthread_mutex_t	no_eat_mutex;
	pthread_t		thread;
	t_table			*data;
}	t_philo;

/* libft functions */
int		ft_atoi(const char *str);
char	*ft_itoa(int n);
void	ft_bzero(void *s, size_t n);
size_t	ft_strlen(const char *str);
int		ft_printerror(char *errormsg);

/* destructor.c */
void	ph_destroy_single(void *arr);
void	ph_destructor(t_philo *philo, t_table *data);

/* error.c */
int		ph_error_check(t_table *data, int ac, char **av);

/* init.c */
void	ph_data_init(t_table *data, char **av, int ac);
t_philo	*ph_init_philos(t_table *data);
void	ph_init_death(t_table *data, t_philo *philo);

/* ph_helper1.c */
char	*ph_message(int reason, int *state);
int		ph_talk(t_philo *philo, int reason);
int		ph_get_current_time(long start);
int		ph_check_state(t_philo *philo);
int		ph_check_death(t_philo *philo);

/* ph_helper2.c */
int		ph_rw_no_eat(t_philo *philo, int rw);

/* main.c */
void	*ph_dinner(void *arg);
void	*ph_death(void *arg);

#endif
