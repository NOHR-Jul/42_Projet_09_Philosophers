/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_meal.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juchene <juchene@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:48:03 by juchene           #+#    #+#             */
/*   Updated: 2023/02/08 14:48:56 by juchene          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

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

int	ph_check_meals(t_main *main)
{
	int			i;
	long int	ref;

	i = -1;
	ref = 0;
	pthread_mutex_lock(&main->status_lock);
	while (++i < main->nbr_phil)
	{
		if (ph_nb_meal(&main->philos[i], 0) == main->philos[i].min_meal)
		{
			ref++;
			if (ref == main->nbr_phil - 1)
			{
				main->mn_status = 2;
				pthread_mutex_unlock(&main->status_lock);
				return (1);
			}
		}			
	}
	pthread_mutex_unlock(&main->status_lock);
	return (0);
}

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
