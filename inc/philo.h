/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juchene <juchene@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 12:50:05 by juchene           #+#    #+#             */
/*   Updated: 2023/02/09 11:12:17 by juchene          ###   ########.fr       */
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

// parsing args :	/!\ if nb must be an integer > 0

// 	- number_of_philosophers
// 	- time_to_die
// 	- time_to_eat
// 	- time_to_sleep
// 	- number_of_times_each_philosopher_must_eat (optionnel)

// Calcul du temps de reflection :

// 	time_to_think = 
//	(time_to_die - (get_time_in_ms() - philo->last_meal) - time_to_eat) / 2

// usleep pour les philosophes avec un identifiant impair (ou pair)

// number_of_philosophers : Le nombre de philosophes, mais aussi le nombre
// de fourchettes.

// time_to_die (en millisecondes) : 
//	Si un philosophe n’a pas commencé à manger time_to_die millisecondes
//	après le début de son précédent repas ou depuis le début de la 
//	simulation, il meurt.

// time_to_eat (en millisecondes) : 
//	Le temps qu’un philosophe prend à manger.
//	Pendant ce temps, un philosophe doit garder ses deux fourchettes.

// number_of_times_each_philosopher_must_eat (argument optionnel) :
//	Si tous les philosophes ont mangé au moins 
//	number_of_times_each_philosopher_must_eat fois, 
//	la simulation prend fin. Si cet argument n’est pas spécifié, alors 
//	la simulation prend fin à la mort d’un philosophe.

// Chaque philosophe se voit assigner un numéro entre 1 
// et number_of_philosophers.

// Le philosophe numéro 1 est assis à côté du philosophe numéro 
// number_of_philosophers.
// Les autres suivent cette logique : philosophe numéro N est assis 
// entre philosophe
// numéro N - 1 et philosophe numéro N + 1.

// Tout changement d’état d’un philosophe doit être formatté comme suit :

// 	◦ timestamp actuel en millisecondes  numéro de philosophe has taken a fork
// 	◦ timestamp actuel en millisecondes  numéro de philosophe is eating
// 	◦ timestamp actuel en millisecondes  numéro de philosophe is sleeping
// 	◦ timestamp actuel en millisecondes  numéro de philosophe is thinking
// 	◦ timestamp actuel en millisecondes  numéro de philosophe died

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
	int				min_meal;
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
	int				min_meal;
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
void		ph_sleeping(t_philo *philo_s);
void		ph_thinking(t_philo *philo_s);
void		ph_log(t_philo *philo_s, char *msg);
void		*routine(void *philo_s);

//	**	ph_process.c	**	//
void		ph_usleep(t_main *main, time_t sleep_time);
int			ph_main_status(t_main *main, int ref);
void		ph_check_death(t_main *main);
int			ph_thread_launch(t_philo *philo);
void		ph_process(t_main *main_s);

#endif
