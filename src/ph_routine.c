/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juchene <juchene@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:50:30 by juchene           #+#    #+#             */
/*   Updated: 2023/02/09 13:57:27 by juchene          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	ph_eating(t_philo *philo_s)	// Nouvelle version
{
	int	ret;

	ret = ph_take_forks(philo_s);
	if (ret)
		return (1);
	if (!ph_main_status(philo_s->p_mn, 0))
	{
		ph_log(philo_s, EAT);
		ph_last_meal(philo_s, get_time_in_ms());
		ph_usleep(philo_s->p_mn, philo_s->time_to_eat);
		ph_nb_meal(philo_s, 1);
	}
	ph_return_forks(philo_s);
	return (0);
}

// int	ph_eating2(t_philo *philo_s)	// A modifier
// {
// 	int	ret;

// 	ret = ph_take_forks(philo_s);
// 	if (ret)
// 		return (1);
// 	ph_log(philo_s, EAT);
// 	ph_last_meal(philo_s, get_time_in_ms());
// 	ph_usleep(philo_s->p_mn, philo_s->time_to_eat);
// 	ph_nb_meal(philo_s, 1);
// 	ph_return_forks(philo_s);
// 	return (0);
// }

void	ph_sleeping(t_philo *philo_s)	// Nouvelle version
{
	if (ph_main_status(philo_s->p_mn, 0))
		return ;
	ph_log(philo_s, SLEEP);
	ph_usleep(philo_s->p_mn, philo_s->time_to_sleep);
}

void	ph_thinking(t_philo *philo_s)	// Nouvelle version
{
	if (ph_main_status(philo_s->p_mn, 0))
		return ;
	ph_log(philo_s, THINK);
}

void	ph_log(t_philo *philo_s, char *msg)	// Nouvelle version
{
	long int	time;
	int 		status;
	
	status = ph_main_status(philo_s->p_mn, 0);
	pthread_mutex_lock(philo_s->print_lock);
	time = get_time_in_ms() - *(philo_s)->time;
	if (status != 1 || !ft_strncmp(msg, DIED, 4))
		printf("%ld %d %s\n", time, (philo_s->ph_id), msg);
	// else if (status == 2)
	// 	printf("%ld %d %s\n", time, (philo_s->ph_id), msg);

	
	pthread_mutex_unlock(philo_s->print_lock);
}

// void	ph_log2(t_philo *philo_s, char *msg)	// A modifier
// {
// 	long int	time;

// 	pthread_mutex_lock(philo_s->print_lock);
// 	time = get_time_in_ms() - *(philo_s)->time;
// 	if (!ph_main_status(philo_s->p_mn, 0)
// 		|| !ft_strncmp(msg, DIED, strlen(DIED)))
// 		printf("%ld %d %s\n", time, (philo_s->ph_id), msg);
// 	else if (ph_main_status(philo_s->p_mn, 0) == 2
// 		&& !ft_strncmp(msg, EAT, strlen(EAT)))
// 		printf("%ld %d %s\n", time, (philo_s->ph_id), msg);
// 	else if (ph_main_status(philo_s->p_mn, 0) == 2
// 		&& !ft_strncmp(msg, TAKE_FORK, strlen(TAKE_FORK)))
// 		printf("%ld %d %s\n", time, (philo_s->ph_id), msg);	
// 	pthread_mutex_unlock(philo_s->print_lock);
// }

void	*routine(void *philo_s)
{
	t_philo		*philo;
	time_t		time;
	long int	spend;
	int			ret;

	philo = (t_philo *)philo_s;
	ret = ph_main_status(philo->p_mn, 0);
	while (ret == 0)
	{
		time = get_time_in_ms();
		spend = (ph_last_meal(philo, 0) + philo->time_to_die - time) * 8 / 10;
		if (ph_main_status(philo->p_mn, 0))
			return (NULL);
		if (philo->meal > 0)
			usleep(spend * 1000);
		ret = ph_eating(philo);
		ph_check_meals(philo->p_mn);
		if (philo->meal == philo->min_meal || ph_main_status(philo->p_mn, 0)
			|| ret)
			return (NULL);
		ph_sleeping(philo);
		ph_thinking(philo);
	}
	return (philo_s);
}
