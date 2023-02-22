/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juchene <juchene@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:50:30 by juchene           #+#    #+#             */
/*   Updated: 2023/02/15 22:24:05 by juchene          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

//	Function that manages the display of instructions
void	ph_log(t_philo *philo_s, char *msg)
{
	long int	time;
	int			status;

	status = ph_main_status(philo_s->p_mn, 0);
	if (status != 1 || !ft_strncmp(msg, DIED, 4))
	{
		pthread_mutex_lock(philo_s->print_lock);
		time = get_time_in_ms() - philo_s->p_mn->time;
		printf("%ld %d %s\n", time, (philo_s->ph_id), msg);
		pthread_mutex_unlock(philo_s->print_lock);
	}
}

//	Function that simulates a philosopher's eating instruction
//	Call of ph_main_status() ph_log() ph_usleep()
int	ph_eating(t_philo *philo_s)
{
	int	ret;

	if (!ph_main_status(philo_s->p_mn, 0))
	{
		ret = ph_take_forks(philo_s);
		if (ret)
			return (ret);
		ph_log(philo_s, EAT);
		ph_last_meal(philo_s, get_time_in_ms());
		ph_usleep(philo_s->p_mn, philo_s->time_to_eat);
		ph_nb_meal(philo_s, 1);
		ph_return_forks(philo_s);
	}
	return (0);
}

//	Function that simulates a philosopher's sleep instruction
//	Call of ph_main_status() ph_log() ph_usleep()
int	ph_sleeping(t_philo *philo_s)
{
	if (ph_main_status(philo_s->p_mn, 0) == 1)
		return (1);
	ph_log(philo_s, SLEEP);
	ph_usleep(philo_s->p_mn, philo_s->time_to_sleep);
	return (0);
}

//	Function that simulates a philosopher's thought instruction
//	Call of ph_main_status() ph_log()
int	ph_thinking(t_philo *philo_s)
{
	if (ph_main_status(philo_s->p_mn, 0) == 1)
		return (1);
	ph_log(philo_s, THINK);
	return (0);
}

//	Set of instructions for threads
void	*routine(void *philo_s)
{
	t_philo		*philo;
	long int	spend;
	int			ret;

	philo = (t_philo *)philo_s;
	ret = ph_main_status(philo->p_mn, 0);
	while (ret == 0)
	{
		spend = 1000;
		if (philo->meal > 0 || (philo->meal == 0 && (philo->ph_id % 2)))
			usleep(spend);
		if (ph_main_status(philo->p_mn, 0))
			return (NULL);
		ret = ph_eating(philo);
		ret += ph_main_status(philo->p_mn, 0);
		if (ret)
			return (NULL);
		ret += ph_sleeping(philo);
		ret += ph_thinking(philo);
	}
	return (philo_s);
}
