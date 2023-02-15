/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juchene <juchene@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 12:50:05 by juchene           #+#    #+#             */
/*   Updated: 2023/02/15 12:56:08 by juchene          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <pthread.h>
# include <string.h>

# define ARGS_NB 	"error usage : ./philo needs 5 to 6 arguments\n"
# define ARGS_VAL 	"error usage : invalid arguments\n"
# define TAKE_FORK 	"has taken a fork"
# define EAT		"is eating"
# define SLEEP		"is sleeping"
# define THINK		"is thinking"
# define DIED		"died"

typedef struct s_time
{
	time_t		start_time;
	time_t		end_time;
}				t_time;

typedef struct s_fork
{
	pthread_mutex_t	fork_lock;
}				t_fork;

typedef struct s_philo
{
	pthread_t		ph_th;
	int				ph_id;
	int				ph_status;
	long int		*time;
	long int		time_to_die;
	long int		time_to_eat;
	long int		time_to_sleep;
	long int		last_meal;
	long int		min_meal;
	int				meal;
	t_fork			*forks[2];
	struct s_main	*p_mn;
	pthread_mutex_t	ph_lock;
	pthread_mutex_t	*print_lock;
}				t_philo;

typedef struct s_main
{
	t_time			time_s;
	int				index;
	int				nbr_phil;
	int				mn_status;
	long int		time;
	long int		time_to_die;
	long int		time_to_eat;
	long int		time_to_sleep;
	long int		min_meal;
	t_philo			*philos;
	t_fork			*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	status_lock;
}				t_main;

//	**	philo.c	**	//
void		ph_show_main_s(t_main *main_s);
void		ph_show_tab(char *tittle, char **tab);

//	**	ph_utils.c	**	//
int			ft_print_err(char *str, int code);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ph_atol(char *nbr);
int			ph_arg_val(char **tab);
long int	get_time_in_ms(void);

//	**	ph_init.c	**	//
void		ph_init_philo(t_main *main_s, int i);
int			ph_init_philos_tab(t_main *main_s);
int			ph_init_forks_tab(t_main *main_s);
int			ph_init_main_s(t_main *main_s, int argc, char **argv);

//	**	ph_free.c	**	//
void		ph_free_forks_tab(t_main *main_s);
void		ph_free_main(t_main *main_s);

//	**	ph_meal.c	**	//
long int	ph_last_meal(t_philo *philo, long int ref);
int			ph_nb_meal(t_philo *philo, int ref);
int			ph_check_meals(t_main *main);
int			ph_take_forks(t_philo *philo_s);
int			ph_return_forks(t_philo *philo_s);

//	**	ph_routine.c	**	//
int			ph_eating(t_philo *philo_s);
int			ph_sleeping(t_philo *philo_s);
int			ph_thinking(t_philo *philo_s);
void		ph_log(t_philo *philo_s, char *msg);
void		*routine(void *philo_s);

//	**	ph_process.c	**	//
void		ph_usleep(t_main *main, time_t sleep_time);
int			ph_main_status(t_main *main, int ref);
void		ph_check_death(t_main *main);
int			ph_thread_launch(t_philo *philo);
void		ph_process(t_main *main_s);

#endif
