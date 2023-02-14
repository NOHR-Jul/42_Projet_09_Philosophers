/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_process.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juchene <juchene@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:53:06 by juchene           #+#    #+#             */
/*   Updated: 2023/02/14 18:59:16 by juchene          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

//	Function uslepp with intern status check
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

//	The main status get/set function
//	if ref != 0 the status is set at ref
//	returns the status of main
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

//	Death check, check the condition of a philosopher 
//	if he has not eaten the required number of meals
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

//	Function which create the thread of a philosopher 
//	with the function with which the thread begins its execution
int	ph_thread_launch(t_philo *philo)
{
	ph_last_meal(philo, get_time_in_ms());
	if (pthread_create(&philo->ph_th, NULL, &routine, philo) != 0)
		return (1);
	return (0);
}

//	Function to launch all threads, the death checker and the thread join
//	Call of ph_thread_launch()
//	Call of ph_check_death()
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
