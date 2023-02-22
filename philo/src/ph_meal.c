/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_meal.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juchene <juchene@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:48:03 by juchene           #+#    #+#             */
/*   Updated: 2023/02/15 22:13:09 by juchene          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

//	The philo's last meal get/set function
//	if ref != 0 the last meal is set at ref
//	returns the number of eaten meal
long int	ph_last_meal(t_philo *philo, long int ref)
{
	long int	ret;

	ret = 0;
	pthread_mutex_lock(&philo->ph_lock);
	if (ref)
		philo->last_meal = ref;
	ret = philo->last_meal;
	pthread_mutex_unlock(&philo->ph_lock);
	return (ret);
}

//	The philo's meal get/set function
//	if ref != 0 the meal is incremented of one
//	returns the number of eaten meal
int	ph_nb_meal(t_philo *philo, int ref)
{
	int	ret;

	ret = 0;
	pthread_mutex_lock(&philo->ph_lock);
	if (ref)
		philo->meal += ref;
	ret = philo->meal;
	pthread_mutex_unlock(&philo->ph_lock);
	return (ret);
}

//	Function that checks if the philosophers have eaten the minimum meal
int	ph_check_meals(t_main *main)
{
	int	i;
	int	ref;

	i = -1;
	ref = 0;
	if (main->min_meal == -1)
		return (0);
	pthread_mutex_lock(&main->status_lock);
	while (++i < main->nbr_phil)
	{
		if (ph_nb_meal(&main->philos[i], 0) >= main->min_meal)
		{
			ref++;
			if (ref == main->nbr_phil)
			{
				if (main->mn_status != 1)
					main->mn_status = 2;
				pthread_mutex_unlock(&main->status_lock);
				return (1);
			}
		}
	}
	pthread_mutex_unlock(&main->status_lock);
	return (0);
}

//	Function that manages the lock of fork's mutex
int	ph_take_forks(t_philo *philo_s)
{
	if ((philo_s->ph_id % 2) == 0)
	{
		pthread_mutex_lock(&philo_s->forks[1]->fork_lock);
		ph_log(philo_s, TAKE_FORK);
		pthread_mutex_lock(&philo_s->forks[0]->fork_lock);
		ph_log(philo_s, TAKE_FORK);
	}
	else
	{
		pthread_mutex_lock(&philo_s->forks[0]->fork_lock);
		ph_log(philo_s, TAKE_FORK);
		if (philo_s->p_mn->nbr_phil == 1)
		{
			pthread_mutex_unlock(&philo_s->forks[0]->fork_lock);
			return (1);
		}
		pthread_mutex_lock(&philo_s->forks[1]->fork_lock);
		ph_log(philo_s, TAKE_FORK);
	}
	return (0);
}

//	Function that manages the unlock of fork's mutex
int	ph_return_forks(t_philo *philo_s)
{
	if ((philo_s->ph_id % 2) == 0)
	{
		pthread_mutex_unlock(&philo_s->forks[0]->fork_lock);
		pthread_mutex_unlock(&philo_s->forks[1]->fork_lock);
	}
	else
	{
		pthread_mutex_unlock(&philo_s->forks[1]->fork_lock);
		pthread_mutex_unlock(&philo_s->forks[0]->fork_lock);
	}
	return (0);
}
