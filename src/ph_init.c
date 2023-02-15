/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juchene <juchene@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:36:51 by juchene           #+#    #+#             */
/*   Updated: 2023/02/15 14:02:11 by juchene          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

//	Function to initilize the philosophers struct
void	ph_init_philo(t_main *main_s, int i)
{
	if (pthread_mutex_init(&main_s->philos[i].ph_lock, NULL) != 0)
		return ;
	main_s->philos[i].ph_id = i + 1;
	main_s->philos[i].ph_status = 0;
	main_s->philos[i].time = &main_s->time;
	main_s->philos[i].time_to_die = main_s->time_to_die;
	main_s->philos[i].time_to_eat = main_s->time_to_eat;
	main_s->philos[i].time_to_sleep = main_s->time_to_sleep;
	main_s->philos[i].min_meal = main_s->min_meal;
	main_s->philos[i].meal = 0;
	main_s->philos[i].last_meal = 0;
	main_s->philos[i].forks[0] = &main_s->forks[i];
	if (i == main_s->nbr_phil - 1)
		main_s->philos[i].forks[1] = &main_s->forks[0];
	else if (main_s->nbr_phil == 1)
		main_s->philos[i].forks[1] = NULL;
	else
		main_s->philos[i].forks[1] = &main_s->forks[i + 1];
	main_s->philos[i].print_lock = &main_s->print_lock;
	main_s->philos[i].p_mn = main_s;
}

//	Function to allocate the philosophers tab
//	call of ph_init_philo()
int	ph_init_philos_tab(t_main *main_s)
{
	int	i;

	i = 0;
	main_s->philos = malloc(main_s->nbr_phil * (sizeof(t_philo)));
	if (!main_s->philos)
		return (2);
	while (i < main_s->nbr_phil)
	{
		ph_init_philo(main_s, i);
		i++;
	}
	return (0);
}

//	Function to allocate the fork's tab
//	and initialize fork's mutex
int	ph_init_forks_tab(t_main *main_s)
{
	int	i;

	i = 0;
	main_s->forks = malloc(main_s->nbr_phil * (sizeof(t_fork)));
	if (!main_s->forks)
		return (2);
	while (i < main_s->nbr_phil)
	{
		if (pthread_mutex_init(&main_s->forks[i].fork_lock, NULL) != 0)
			return (2);
		i++;
	}	
	return (0);
}

//	Function to initialize t_main struct
int	ph_init_main_s(t_main *main_s, int argc, char **argv)
{
	if (pthread_mutex_init(&main_s->print_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&main_s->status_lock, NULL) != 0)
		return (2);
	main_s->index = 0;
	main_s->mn_status = 0;
	main_s->time = get_time_in_ms();
	main_s->nbr_phil = ph_atol(argv[1]);
	main_s->time_to_die = ph_atol(argv[2]);
	main_s->time_to_eat = ph_atol(argv[3]);
	main_s->time_to_sleep = ph_atol(argv[4]);
	main_s->min_meal = -1;
	if (argc > 5)
		main_s->min_meal = ph_atol(argv[5]);
	ph_init_forks_tab(main_s);
	ph_init_philos_tab(main_s);
	return (0);
}
