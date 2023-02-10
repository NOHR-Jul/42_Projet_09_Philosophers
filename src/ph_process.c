/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_process.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juchene <juchene@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:53:06 by juchene           #+#    #+#             */
/*   Updated: 2023/02/10 11:54:30 by juchene          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	ph_usleep(t_main *main, long int sleep_time)
{
	long int	time;
	long int	t_now;
	long int	wake_up;

	time = get_time_in_ms();
	t_now = time;
	wake_up = t_now + sleep_time;
	while (t_now <= wake_up)
	{
		usleep(100);
		t_now = get_time_in_ms();
		if (ph_main_status(main, 0))
			break ;
	}
}

int	ph_main_status(t_main *main, int ref)
{
	int	ret;

	ret = 0;
	pthread_mutex_lock(&main->status_lock);
	if (ref)
		main->mn_status = ref;
	ret = main->mn_status;
	pthread_mutex_unlock(&main->status_lock);
	return (ret);
}

// int	ph_main_status2(t_main *main, int ref)
// {
// 	int	ret;

// 	ret = 0;
// 	ph_check_meals(main);
// 	pthread_mutex_lock(&main->status_lock);
// 	if (ref)
// 		main->mn_status = ref;
// 	ret = main->mn_status;
// 	pthread_mutex_unlock(&main->status_lock);
// 	return (ret);
// }

void	ph_check_death(t_main *main)
{
	int			i;
	long int	ref;

	while (1)
	{
		i = -1;
		while (++i < main->nbr_phil)
		{
			if (ph_nb_meal(&main->philos[i], 0) == main->philos[i].min_meal)
				continue ;
			ref = (get_time_in_ms() - ph_last_meal(&main->philos[i], 0));
			if (ref >= (main->philos[i].time_to_die))
			{
				ph_main_status(main, 1);
				ph_log(&main->philos[i], DIED);
				break ;
			}
		}
		ref = ph_main_status(main, 0);
		if (ref || ph_check_meals(main))
			return ;
		usleep(500);
	}	
}

int	ph_thread_launch(t_philo *philo)
{
	ph_last_meal(philo, get_time_in_ms());
	if (pthread_create(&philo->ph_th, NULL, &routine, philo) != 0)
		return (1);
	return (0);
}

void	ph_process(t_main *main_s)
{
	int	i;

	i = 0;
	while (i < main_s->nbr_phil)
	{
		if (ph_thread_launch(&main_s->philos[i]))
			return ;
		i += 2;
	}
	usleep(80);
	i = 1;
	while (i < main_s->nbr_phil)
	{
		if (ph_thread_launch(&main_s->philos[i]))
			return ;
		i += 2;
	}
	usleep(100);
	ph_check_death(main_s);
	usleep(200);
	i = main_s->nbr_phil;
	while (i-- > 0)
		pthread_join(main_s->philos[i].ph_th, NULL);
}
